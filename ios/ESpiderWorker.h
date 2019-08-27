//
//  ESpiderThread.h
//  RNEspider
//
//  Created by Suhan Moon on 28/02/2019.
//  Copyright © 2019 Facebook. All rights reserved.
//

#import <React/RCTEventEmitter.h>

#import "include/SpiderEngine.h"
#import "JSON/JSON.h"

NS_ASSUME_NONNULL_BEGIN

@interface ESpiderWorker : NSObject <SpiderEngineDelegate>
    
- (id) initWithHandler:(RCTEventEmitter*) handler;
    
- (void)runJobs:(NSMutableArray *)jobs resolver:(RCTPromiseResolveBlock)resolve rejecter:(RCTPromiseRejectBlock)reject;
- (void)dispose;

@end

NS_ASSUME_NONNULL_END
