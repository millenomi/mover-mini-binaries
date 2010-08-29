

#import <Foundation/Foundation.h>

/**
 Represents another device on the network that Mover Mini can send to or receive from. Endpoint objects are provided by the MvrEngine as they're discovered on the network.
 */
@protocol MvrEndpoint <NSObject>

/** The display name for this endpoint. */
- (NSString*) displayName;

@end

