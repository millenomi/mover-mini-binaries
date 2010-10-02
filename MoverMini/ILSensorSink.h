//
//  ILSensorSink.h
//  TheLongMix
//
//  Created by ∞ on 29/09/10.
//  Copyright 2010 Emanuele Vulcano (Infinite Labs). All rights reserved.
//

#import <Foundation/Foundation.h>

@class ILSensorSink;

/**
 Debug only: A sensor sink tap can receive all logging messages that are sent to a ILSensorSink. You can create a sink tap to perform special handling of logging messages and to prevent them from being displayed on the standard sink output.
 */
@protocol ILSensorSinkTap <NSObject>

/**
 Handles a message coming from the sensor sink.
 
 @param s The sensor sink this message was sent to.
 @param channels A set of strings, the channels this message was sent to. (For more information, see ILSensorSink#logMessageWithContent:forChannels:.)
 @param content A JSON-like fragment, consisting of either a NSString, NSNumber, NSNull object, NSArray or NSDictionary. NSArrays will only contain objects of the aforementioned classes, whereas NSDictionaries will only contain objects of the aforementioned classes as values and NSStrings as keys.
 
 @return YES if you want to allow the sensor sink to further handle the message; NO if you have fully handled it. (In this version of Telemetry, the sensor sink's handling of a message prints a summary of that message to the console via NSLog.)
 */
- (BOOL) sensorSink:(ILSensorSink*) s didReceiveMessageOnChannels:(NSSet*) channels withContent:(id) content;

@end

/**
 Debug only: A sensor sink is the ultimate destination of all messages produced by telemetry-enabled classes. You can send messages to a sink manually, through utility macros (such as @ref ILLog and @ref ILLogDict), or handle logging messages yourself by setting a tap for this sensor sink (using the #tap property).
 */
@interface ILSensorSink : NSObject {}

/** Returns the shared sensor sink instance. */
+ sharedSink;

/**
 Sends a telemetry message.
 
 @param content The content of the message. This can be any object. The sensor sink will use this object to produce an informative JSON-like payload (consisting of NSArray, NSDictionary, NSString, NSNumber and NSNull objects only), using the NSObject#description and NSObject#descriptionForDebugging methods if needed.
 @param channels A set of strings, each of which is a channel upon which the message is being sent. Channels can be used to only display or handle messages coming from certain sources.
 */
- (void) logMessageWithContent:(id) content forChannels:(NSSet*) channels;

/**
 @private
 This method is part of the implementation of @ref ILLog and @ref ILLogDict and should not be called directly.
 */
+ (void) log:(id) content atLine:(unsigned long long) line function:(const char*) functionName object:(id) object channel:(NSString*) channel;

/**
 @private
 This method is part of the implementation of telemetry internals and should not be called directly.
 */
+ (id) transferableObjectForObject:(id) object;

/**
 Sets or retrieves the sensor tap for this sink. If non-nil, the tap will receive all messages before 
 */
@property(retain, nonatomic) id <ILSensorSinkTap> tap;

@end

/** Sends a telemetry message whose content is a dictionary. You pass values and keys of the dictionary (similar to NSDictionary#dictionaryWithObjectsAndKeys:), except no nil marker is needed at the end.
 */
#define ILLogDict(...) \
	[ILSensorSink log:([NSDictionary dictionaryWithObjectsAndKeys:__VA_ARGS__, nil]) atLine:__LINE__ function:__PRETTY_FUNCTION__ object:(self) channel:(nil)]

/** Sends a telemetry message whose content is a formatted string. This macro is a drop-in replacement for NSLog.
 */
#define ILLog(x, ...) \
	[ILSensorSink log:([NSString stringWithFormat:(x) , ## __VA_ARGS__]) atLine:__LINE__ function:__PRETTY_FUNCTION__ object:(self) channel:(nil)]


@interface NSObject (ILSensorDebuggingDescription)

/**
 If implemented, ILSensorSink will use the return value of this method as the JSON fragment describing this object. It can be a NSString, NSNumber, NSNull object, or a NSArray of such objects, or a NSDictionary of NSString keys and any such objects.
 */
- (id) descriptionForDebugging;

@end
