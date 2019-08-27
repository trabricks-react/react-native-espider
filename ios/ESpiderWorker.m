//
//  ESpiderThread.m
//  RNEspider
//
//  Created by Suhan Moon on 28/02/2019.
//  Copyright © 2019 Facebook. All rights reserved.
//

#import "ESpiderWorker.h"

@implementation ESpiderWorker {
    SpiderEngine *spiderEngine;
    NSMutableArray *_jobs;
    NSMutableArray *_success;
    
    RCTEventEmitter* _handler;
    
    int _size;
    
    RCTPromiseResolveBlock _resolve;
    RCTPromiseRejectBlock _reject;
}


- (id) initWithHandler:(RCTEventEmitter*) handler {
    self = [super init];
    if (self) {
        _handler = handler;
    }
    return self;
    
}


- (void)dispose {
    if (spiderEngine != nil) {
        [spiderEngine stopEngine];
        spiderEngine.delegate = nil;
        spiderEngine = nil;
    }
}

- (void)dealloc {
    [self dispose];
}

- (void)runJobs:(NSMutableArray *)jobs
       resolver:(RCTPromiseResolveBlock)resolve rejecter:(RCTPromiseRejectBlock)reject {
    
    _size = 0;
    _jobs = [[NSMutableArray alloc] initWithArray: jobs];
    _resolve = resolve;
    _reject = reject;
    
    _success = [NSMutableArray array];
    
    spiderEngine = [[SpiderEngine alloc] init];
    spiderEngine.licenseKey     = [NSString stringWithFormat:@"%@", [[NSBundle mainBundle] objectForInfoDictionaryKey:@"E_SPIDER_KEY"]];  // e-spider 라이센스 키 입력
    spiderEngine.delegate       = self;                                     // delegate 설정
    spiderEngine.threadCount    = 8;                                        // 모듈 thread 갯수입력 (최대 8개 가능)
    spiderEngine.autoStop       = YES;

    if ([spiderEngine startEngine] == NO || [spiderEngine startJob] == NO) {
        if (_resolve != nil) {
            reject(@"RNESpider", @"작업이 실패하였습니다.", nil);
            _resolve = nil;
            _reject = nil;
        }
        return;
    }
}


/* number of job count */
- (NSUInteger)numberOfJobInEngine:(SpiderEngine *)engine {
    return [_jobs count];
}


/* Module data callback */
- (NSDictionary *)engine:(SpiderEngine *)engine jobDataForJobIndex:(NSUInteger)jobIndex {
    return [[_jobs objectAtIndex:jobIndex] objectForKey:ENGINE_JOB_MODULE_KEY];
}
// Heenam // 02 - 6933 - 5358

/* Module get Parameter callback */
- (NSString *)engine:(SpiderEngine *)engine
         threadIndex:(NSUInteger)threadIndex
            jobIndex:(NSUInteger)jobIndex
   requireJSONString:(NSString *)requireJSONString
         synchronous:(BOOL)synchronous {

    /*
     모듈이 호출될 상세정보를 넘겨주는 delegate 입니다.
     asynchronous 와 synchronous 를 판단하여 추가 입력 파라메터가 필요할시(화면에 입력받는 파라메터)
     정보를 넘겨줄 수 있는 함수입니다.
     */

    if (!requireJSONString) return NULL;

    NSDictionary *requireJson = (NSDictionary *)[[NSString stringWithFormat:@"%@", requireJSONString] JSONValue];
    if (synchronous) {
        NSArray *keys = [requireJson allKeys];
        NSDictionary *dicInfo;
        NSDictionary *dicElemtry;
        NSMutableDictionary *dicRes = [NSMutableDictionary dictionaryWithCapacity:1];

        for (NSString *keyName in keys) {
            dicInfo = [[_jobs objectAtIndex:jobIndex] objectForKey:keyName];

            dicElemtry = [requireJson objectForKey:keyName];
            NSArray *reqKeys = [dicElemtry allKeys];
            NSMutableDictionary *dicReturn= [NSMutableDictionary dictionaryWithCapacity:[reqKeys count]];

            for (NSString *reqKeyName in reqKeys) {
                id value = [dicInfo objectForKey:reqKeyName];
                if (value) {
                    [dicReturn setObject:value forKey:reqKeyName];
                }
            }

            [dicRes setObject:dicReturn forKey:keyName];
        }

        if (dicRes){
            if (_handler != nil) [_handler sendEventWithName:@"WorkStart" body: [_jobs objectAtIndex:jobIndex]];
            return [dicRes JSONFragment];
        }
        else
            return @"";
    }

    return nil;
}


/**
 * Module result callback
 *
 * @param engine SpiderEngine
 * @param threadIndex {NSUInteger} thread index
 * @param jobIndex {NSUInteger} add job index
 * @param error {NSUInteger} if eroor is not 0 then occure error else success
 * @param userError {NSString} user error is site error code
 * @param errorMessage {NSString} user error is site error message
 * @param resultJsonString {NSString} if error is 0 then module result string (json string) else (null or "")
 * @since espider1.0 and later.
 */
- (void)engine:(SpiderEngine *)engine
   threadIndex:(NSUInteger)threadIndex
      jobIndex:(NSUInteger)jobIndex
         error:(NSUInteger)error userError:(NSString *)userError
  errorMessage:(NSString *)errorMessage resultJsonString:(NSString *)resultJsonString {


    /*
     하나의 모듈이 완료가 되면 처리 데이터를 전달하는 delegate 입니다.
     모듈 에러시에 같이 처리 가능합니다.
     */
    @try {
        NSMutableDictionary* curr = [_jobs objectAtIndex:jobIndex];
        if (error == 0) {
            NSMutableDictionary* row = [NSMutableDictionary dictionary];
            [row setObject:@YES                         forKey:@"success"];
            [row setObject:[resultJsonString JSONValue] forKey:@"data"];
            [row setObject:[curr objectForKey:ENGINE_JOB_MODULE_KEY] forKey:ENGINE_JOB_MODULE_KEY];
            [_success addObject: row];
            if (_handler != nil) [_handler sendEventWithName:@"WorkFinish" body: row];
        }
        else {
            NSMutableDictionary* row = [NSMutableDictionary dictionary];
            [row setObject:@NO          forKey:@"success"];
            [row setObject:userError    forKey:@"userError"];
            [row setObject:errorMessage forKey:@"errorMessage"];
            [row setObject:[NSNumber numberWithUnsignedInteger: error & 0xfff] forKey:@"error"];
            [row setObject:[curr objectForKey:ENGINE_JOB_MODULE_KEY] forKey:ENGINE_JOB_MODULE_KEY];
            [_success addObject: row];
            if (_handler != nil) [_handler sendEventWithName:@"WorkFinish" body: row];
        }
        
        
        if (_size >= 0) _size++;
        if ([_jobs count] <= _size) {
            _size = -100;
            
            if (_resolve != nil) {
                _resolve(_success);
            }
            _resolve = nil;
            _reject = nil;
            
            [self dispose];
        }
    }
    @catch(NSException * e) {
        NSLog(@"%@", e);
        _reject(@"RNESpider", @"작업이 실패하였습니다.", nil);

        _resolve = nil;
        _reject = nil;
    }
    
}


/**
 * Module result callback
 *
 * Engine Error is not config or system error
 *
 * @param engine SpiderEngine
 * @param engineError {NSError} engine system error
 * @since espider1.0 and later.
 */
- (void)engine:(SpiderEngine *)engine engineError:(NSError *)error {
    /*
     모듈에러시에 진입되는 delegate 입니다.
     */
    
    NSLog(@" Module result callback Error : [%ld]", (long)(error.code & 0xfff));
}


- (void)engine:(SpiderEngine *)engine status:(NSUInteger)status {
    /*
     작업이 완료되면 진입되는 delegate 입니다.
     status = 0 이면 작업이 완료된 상태입니다.
     페이지 이동 및 메세지 처리시 performSelectorOnMainThread 에서 꼭 처리하셔야 합니다.
     */
    
    NSLog(@" Engine status call back  : [%lu]", (unsigned long)status);
    
    // 작업이 완료되면 다음 처리
    if (status == 0) {
    }
}

@end
