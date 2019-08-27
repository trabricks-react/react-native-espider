#import "RNEspider.h"

@implementation RNEspider

- (dispatch_queue_t)methodQueue
{
    return dispatch_queue_create("com.tudal.tp.ESpider", DISPATCH_QUEUE_SERIAL);
}

RCT_EXPORT_MODULE();

- (NSArray<NSString *> *)supportedEvents
{
    return @[@"WorkFinish", @"WorkStart"];
}
    
    //cancelJob

RCT_REMAP_METHOD(cancelJob, cancelJob: (RCTPromiseResolveBlock)resolve rejecter: (RCTPromiseRejectBlock)reject) {
    if (_worker != nil) {
        [_worker dispose];
        _worker = nil;
    }
}


RCT_REMAP_METHOD(execute, execute: (NSDictionary*)jobInfo
                 resolver: (RCTPromiseResolveBlock)resolve rejecter: (RCTPromiseRejectBlock)reject) {
    
    NSMutableArray* jobs = [[NSMutableArray alloc] init];
    [jobs addObject:jobInfo];
    
    if (_worker != nil) {
        [_worker dispose];
        _worker = nil;
    }
    
    ESpiderWorker* worker = [[ESpiderWorker alloc] initWithHandler: self];
    [worker runJobs:jobs resolver:resolve rejecter:reject];
    _worker = worker;
}


RCT_REMAP_METHOD(executes, execute2: (NSArray*)input
                 resolver: (RCTPromiseResolveBlock)resolve rejecter: (RCTPromiseRejectBlock)reject) {
    
    NSMutableArray* jobs = [[NSMutableArray alloc] initWithArray: input];
    if (_worker != nil) {
        [_worker dispose];
        _worker = nil;
    }
    
    ESpiderWorker* worker = [[ESpiderWorker alloc] initWithHandler: self];
    [worker runJobs:jobs resolver:resolve rejecter:reject];
    _worker = worker;
}

@end 
