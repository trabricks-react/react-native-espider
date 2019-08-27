//
//  SpiderEngine.h
//  e-spider
//
//  Created by htjulia on 6/24/14.
//  Copyright (c) 2014 Heenam. All rights reserved.
//

#import <Foundation/Foundation.h>

#pragma mark
#pragma mark engine use key
#pragma mark

//Main Parameter key Define
#define ENGINE_JOB_MODULE_KEY               @"module"
#define ENGINE_JOB_PARAM_LOGIN_KEY          @"loginInfo"
#define ENGINE_JOB_PARAM_INFO_KEY           @"paramInfo"
#define ENGINE_JOB_PARAMEXT_INFO_KEY        @"paramExtInfo"

//ENGINE_JOB_MODULE_KEY info subkeys
#define MODULE_CODE_COUNTRY_KEY             @"country"                  //A2CODE
#define MODULE_CODE_ORGANIZATION_KEY        @"organization"             //O2CODE
#define MODULE_CODE_SUBORGANIZATION_KEY     @"suborganization"          //S4CODE
#define MODULE_CODE_MODULECODE_KEY          @"code"                     //M6CODE


//ENGINE_JOB_PARAM_LOGIN_KEY info subkeys
//#define LOGIN_REQUEST_USERID_KEY            @"reqUserId"              //deprecated
//#define LOGIN_REQUEST_USERPASSWORD_KEY      @"reqUserPass"            //deprecated
//#define LOGIN_REQUEST_CERT_FILE_KEY         @"reqCertFile"            //deprecated
//#define LOGIN_REQUEST_CERTKEY_FILE_KEY      @"reqKeyFile"             //deprecated
//#define LOGIN_REQUEST_CERTKEY_PASSWORD_KEY  @"reqCertPass"            //deprecated


#define ENGINE_UPDATE_DOMAIN1               "http://update1.espider.co.kr"
#define ENGINE_UPDATE_DOMAIN2               "http://update2.espider.co.kr"



//device infomation key list since 2.1.0
#define ENGINE_DEVICE_APP_ID                @"appId"
#define ENGINE_DEVICE_APP_VERSION           @"appVersion"
#define ENGINE_DEVICE_UNIQUE_ID             @"deviceId"
#define ENGINE_DEVICE_UUID                  @"deviceUUID"
#define ENGINE_DEVICE_MANUFACTURER          @"deviceManufacturer"
#define ENGINE_DEVICE_MODEL                 @"deviceModel"
#define ENGINE_DEVICE_OS_NAME               @"osName"
#define ENGINE_DEVICE_OS_VERSION            @"osVersion"
#define ENGINE_DEVICE_PLATFORM              @"platform"
#define ENGINE_DEVICE_PLATFORM_NAME         @"platformName"
#define ENGINE_DEVICE_USER_NAME             @"deviceName"
#define ENGINE_DEVICE_LOCALE_NAME           @"deviceLocale"
#define ENGINE_DEVICE_INFO_UNKNOWN          @"unknown"
#define ENGINE_DEVICE_B64_UNKNOWN           @"dW5rbm93bg=="     //since 2.1.5


@protocol SpiderEngineDelegate;


#pragma mark
#pragma mark SpiderEngine interface
#pragma mark
/**
 * @enum SE_ENGINE_STATUS
 */
typedef enum {
    SE_ENGINE_STATUS_IDLE           = 0,        //engine idle status
    SE_ENGINE_STATUS_INITIALIZED    = 1,        //engine initilazied status
    SE_ENGINE_STATUS_RUNNING        = 2,        //engine running status
    SE_ENGINE_STATUS_SUSPENDING     = 3,        //engine suppending
    SE_ENGINE_STATUS_SUSPENDED      = 4,        //engine suppended
    SE_ENGINE_STATUS_CANCEL         = 5,        //engine canceling or cancel
    SE_ENGINE_STATUS_DONE           = 6,        //engine status done
}  SE_ENGINE_STATUS;



/** 
 * @enum SE_ENGINE_JOB_STATUS
 */
typedef enum {
    SE_ENGINE_JOB_PREPARE 			= 0,        //job status prepare
    SE_ENGINE_JOB_INIT 				= 1,        //job status initialization
    SE_ENGINE_JOB_RESERVED0			= 2,        //job status reserved (do not using)
    SE_ENGINE_JOB_GETPARAM 			= 3,        //job status get param
    SE_ENGINE_JOB_SSO_RELOCATION    = 4,        //job status SSO relocation (change thread index)
    SE_ENGINE_JOB_LOGIN 			= 5,        //job status login
    SE_ENGINE_JOB_RESERVED2			= 6,        //job status reserved (do not using)
    SE_ENGINE_JOB_GETDATA 			= 7,        //job status get data
    SE_ENGINE_JOB_RESERVED3			= 8,        //job status reserved (do not using)
    SE_ENGINE_JOB_CONVERT 			= 9,        //job status data convert
    SE_ENGINE_JOB_RESERVED4			= 10,       //job status reserved (do not using)
    SE_ENGINE_JOB_SETRESULT			= 11,       //job status result
    SE_ENGINE_JOB_RESERVED5			= 12,       //job status reserved (do not using)
    SE_ENGINE_JOB_RESERVED6			= 13,       //job status reserved (do not using)
    SE_ENGINE_JOB_RESERVED7			= 14,       //job status reserved (do not using)
    SE_ENGINE_JOB_END 				= 15,       //job status done
} SE_ENGINE_JOB_STATUS;



/**
 * @enum SE_DIGEST
 */
typedef enum {
    SE_DIGEST_SHA                   = 0,        //SHA DIGEST
    SE_DIGEST_SHA1                  = 1,        //SHA1 DIGEST
    SE_DIGEST_SHA224                = 2,        //SHA224 DIGEST
    SE_DIGEST_SHA256                = 3,        //SHA256 DIGEST
    SE_DIGEST_SHA384                = 4,        //SHA386 DIGEST
    SE_DIGEST_SHA512                = 5,        //SHA512 DIGEST
    SE_DIGEST_MD5                   = 6,        //MD5 DIGEST
}  SE_DIGEST;


/**
 * @enum SE_CIPHER
 */
typedef enum {
    SE_CIPHER_SEED_CBC              = 10,        //SEED-CBC 128
    SE_CIPHER_AES128_CBC            = 11,        //AES128-CBC
}  SE_CIPHER;


/**
 * @enum SE_CIPHER_TYPE
 */
typedef enum {
    SE_CIPHER_TYPE_BASE64           = 0,        //BASE64 encoded
    SE_CIPHER_TYPE_HEXSTRING        = 1,        //Hex String
} SE_CIPHER_TYPE;




@interface SpiderEngine : NSObject

/**
 * @abstract setting SpiderEngineDelegate protocol
 *
 * @since espider1.0 and later.
 */
@property (nonatomic, weak) id<SpiderEngineDelegate> delegate;


/**
 * @abstract setting license key(guid string).
 *
 * @discussion must be set before calling startEngine method.
 * @since espider1.0 and later.
 */
@property (nonatomic, strong) NSString *licenseKey;

/**
 * @abstract threadCount is setting thread pool
 *
 * @discussion must be set before calling startEngine method. default 1(thread count set ios:1~4 osx:unlimited)
 * @since espider1.0 and later.
 */
@property (nonatomic) NSUInteger threadCount;


/**
 * @abstract setting autostop {bool}
 *
 * @discussion engine is stop automatically when engine job done(when success).
 * @since espider1.0 and later.
 */
@property (nonatomic) BOOL autoStop;


/**
 * @abstract status is current engine status @see SE_ENGINE_STATUS
 *
 * @since espider1.0 and later.
 */
@property (nonatomic) SE_ENGINE_STATUS status;


/**
 * @abstract version is current engine version {NSString}
 *
 * @since espider1.1 and later.
 */
@property (nonatomic, readonly)NSString *version;


/**
 * @abstract e-spider sharedEngine is sigleton class method
 *
 * @return engine {SpiderEngine}
 * @since espider1.0 and later.
 */
+ (id)sharedEngine;


/**
 * @abstract e-spider startEngine
 *
 * @return {bool} call function success
 * @since espider1.0 and later.
 */
- (BOOL)startEngine;


/**
 * @abstract e-spider startJob
 *
 * @return {bool} call function success
 * @since espider1.0 and later.
 */
- (BOOL)startJob;


/**
 * @abstract e-spider stopEngine
 *
 * @return {bool} call function success
 * @since espider1.0 and later.
 */
- (BOOL)stopEngine;


/**
 * @abstract e-spider cancelAll
 *
 * @discussion running job waiting until the end. stand by job is cancel.
 * @return {bool} call function success
 * @since espider1.0 and later.
 */
- (BOOL)cancelAll;



/**
 * @abstract e-spider cancelAllForce
 *
 * @discussion running job is cancel forced. standby job is cancel.
 * @return {bool} call function success
 * @since espider1.0 and later.
 */
- (BOOL)cancelAllForce;



/**
 * @abstract e-spider cancelJobThread
 *
 * @discussion running thread one job is cancel forced. standby job is not cancel.
 * @param threadIndex {NSUInteger} thread index
 * @return {bool} call function success
 * @since espider1.0 and later.
 */
- (BOOL)cancelJobThread:(NSUInteger)threadIndex;


/**
 * @abstract e-spider thread suspend
 *
 * @return {bool} call function success
 * @since espider1.0 and later.
 */
- (BOOL)suspend;


/**
 * @abstract e-spider thread resume
 *
 * @return {bool} call function success
 * @since espider1.0 and later.
 */
- (BOOL)resume;


/**
 * @abstract e-spider a-synchroized setting parameter data
 *
 * @discussion engine callback engine:threadIndex:jobIndex:requireJSONString:synchronous: synchronous is false. must send this method.
 * @param threadIdx {NSUInteger} thread index
 * @param jobIdx {NSUInteger} job index
 * @param paramJsonString {NSString} parameter Json string
 * @since espider1.0 and later.
 */
- (void)setParamData:(NSUInteger)threadIdx jobIdx:(NSUInteger)jobIdx paramJsonString:(NSString *)paramJsonString;



/**
 * @abstract e-spider encrypt option algorism
 *
 * @discussion
 * @param options {BOOL} enable option.
 * @param randomHex {NSString} random hex String.
 * @param hashAlgorism {SE_DIGEST} hashAlgorism see enum SE_DIGEST.
 * @param cipherAlgorism {SE_CIPHER} cipherAlgorism see enum SE_CIPHER.
 * @param saltHex {NSString} saltHex hex String.
 * @param iterator {NSUInteger} iterator count
 * @param digestLen {NSUInteger} digest lenth
 * @param cipherIVHex {NSString} cipherIV hex string
 * @param padding {BOOL} padding on off
 * @param cipherDataType {SE_CIPHER_TYPE} cipher input Data Type
 * @return {bool} setting valid true.
 * @since espider1.1 and later.
 */
- (BOOL)setOptions:(BOOL)enable
         randomHex:(NSString *)randomHex
      hashAlgorism:(SE_DIGEST)hashAlgorism
    cipherAlgorism:(SE_CIPHER)cipherAlgorism
           saltHex:(NSString *)saltHex
          iterator:(NSUInteger)iterator
       cipherIVHex:(NSString *)cipherIVHex
           padding:(BOOL)padding
    cipherDataType:(SE_CIPHER_TYPE)cipherDataType;



/**
 * @abstract e-spider encrypt option algorism
 *
 * @discussion
 * @param options {BOOL} enable option.
 * @param cipherKeyHex {NSString} cipher key hex String.
 * @param cipherIVHex {NSString} cipherIV hex string
 * @param padding {BOOL} padding on off
 * @param cipherAlgorism {SE_CIPHER} cipherAlgorism see enum SE_CIPHER.
 * @param cipherDataType {SE_CIPHER_TYPE} cipher input Data Type
 * @return {bool} setting valid true.
 * @since espider1.2.6 and later.
 */
- (BOOL)setOptionsRaon:(BOOL)enable
          cipherKeyHex:(NSString *)cipherKeyHex
           cipherIVHex:(NSString *)cipherIVHex
               padding:(BOOL)padding
        cipherAlgorism:(SE_CIPHER)cipherAlgorism
        cipherDataType:(SE_CIPHER_TYPE)cipherDataType;


/**
 * @abstract e-spider encrypt option algorism
 *
 * @discussion
 * @param enable {BOOL} enable option. decrypt data on
 * @param publicKeyString {NSString} publicKeyString String.
 * @param coworkCode {NSString} coworkCode String.
 * @param hashAlgorism {SE_DIGEST} hashAlgorism see enum SE_DIGEST.
 * @param cipherDataType {SE_CIPHER_TYPE} cipher input Data Type
 * @return {bool} setting valid true.
 * @since espider1.2.1 and later.
 */
- (BOOL)setOptionsNFilter:(BOOL)enable
          publicKeyString:(NSString *)publicKeyString
               coworkCode:(NSString *)coworkCode
             hashAlgorism:(SE_DIGEST)hashAlgorism
           cipherDataType:(SE_CIPHER_TYPE)cipherDataType;



/**
 * @abstract e-spider encrypt option algorism
 *
 * @discussion
 * @param options {BOOL} enable option.
 * @param cipherKeyHex {NSString} cipher key hex String.
 * @param cipherIVHex {NSString} cipherIV hex string
 * @param padding {BOOL} padding on off
 * @param cipherAlgorism {SE_CIPHER} cipherAlgorism see enum SE_CIPHER.
 * @param cipherDataType {SE_CIPHER_TYPE} cipher input Data Type
 * @return {bool} setting valid true.
 * @since espider1.2.7 and later.
 */
- (BOOL)setOptionsEversafe:(BOOL)enable
              cipherKeyHex:(NSString *)cipherKeyHex
               cipherIVHex:(NSString *)cipherIVHex
                   padding:(BOOL)padding
            cipherAlgorism:(SE_CIPHER)cipherAlgorism
            cipherDataType:(SE_CIPHER_TYPE)cipherDataType;



/**
 * @abstract e-spider get device information
 *
 * @discussion
 * @param infoKey {NSString} device infomation key.
 * @return {NSstring} information value. do not exists is return ""
 * @since espider2.1.0 and later.
 */
- (NSString *)getDeviceInfo:(NSString *)infoKey;


@end



#pragma mark
#pragma mark SpiderEngineDelegate protocol
#pragma mark
@protocol SpiderEngineDelegate <NSObject>
@required
/**
 * number of job count
 *
 * @param engine SpiderEngine
 * @return {NSUInteger} job count
 * @since espider1.0 and later.
 */
- (NSUInteger)numberOfJobInEngine:(SpiderEngine *)engine;


/**
 * Module data callback
 *
 * @param engine SpiderEngine
 * @param jobDataForJobIndex {NSUInteger} Job index
 * @return {NSDictionary} job data
 * @since espider1.0 and later.
 */
- (NSDictionary *)engine:(SpiderEngine *)engine jobDataForJobIndex:(NSUInteger)jobIndex;


/**
 * Module get Parameter callback
 *
 * @param engine SpiderEngine
 * @param threadIndex {NSUInteger} thread index
 * @param jobIndex {NSUInteger} add job index
 * @param requireJSONString {NSString} get parameter string (JSON String)
 * @param synchronous {BOOL} true is synchronized param false a-synchronized param. if synchronous is false then must be call setParamData.
 * @since espider1.0 and later.
 */
- (NSString *)engine:(SpiderEngine *)engine threadIndex:(NSUInteger)threadIndex jobIndex:(NSUInteger)jobIndex requireJSONString:(NSString *)requireJSONString synchronous:(BOOL)synchronous;


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
- (void)engine:(SpiderEngine *)engine threadIndex:(NSUInteger)threadIndex jobIndex:(NSUInteger)jobIndex error:(NSUInteger)error userError:(NSString *)userError errorMessage:(NSString *)errorMessage resultJsonString:(NSString *)resultJsonString;


/**
 * Module result callback
 *
 * Engine Error is not config or system error
 *
 * @param engine SpiderEngine
 * @param engineError {NSError} engine system error
 * @since espider1.0 and later.
 */
- (void)engine:(SpiderEngine *)engine engineError:(NSError *)error;


@optional
/**
 * Engine status call back
 *
 * @param engine SpiderEngine
 * @param status {NSUInteger} status code
 * @since espider1.0 and later.
 */
- (void)engine:(SpiderEngine *)engine status:(NSUInteger)status;

/**
 * Module status call back
 *
 * @param engine SpiderEngine
 * @param threadIndex {NSUInteger} thread index
 * @param jobIndex {NSUInteger} add job index
 * @param status {NSUInteger} status code
 * @since espider1.0 and later.
 */
- (void)engine:(SpiderEngine *)engine threadIndex:(NSUInteger)threadIndex jobIndex:(NSUInteger)jobIndex status:(NSUInteger)status;


/**
 * Module progress percent
 *
 * @param engine SpiderEngine
 * @param threadIndex {NSUInteger} thread index
 * @param jobIndex {NSUInteger} add job index
 * @param percent {NSUInteger} percent
 * @since espider1.0 and later.
 */
- (void)engine:(SpiderEngine *)engine threadIndex:(NSUInteger)threadIndex jobIndex:(NSUInteger)jobIndex percent:(NSUInteger)percent;


@end
