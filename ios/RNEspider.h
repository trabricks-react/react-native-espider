#import "ESpiderWorker.h"

@interface RNEspider : RCTEventEmitter <RCTBridgeModule> {
    ESpiderWorker* _worker;
}

@end
  
