
#import <Foundation/Foundation.h>

/** Represents an incoming transfer in progress. You can use this object's methods to query for (and receive updates relating to) the transfer. Each object represents a single transfer; objects conforming to this protocol are never reused.
 
	Values returned by this protocol's methods are only meaningful while the transfer is in progress. Once it ends (as reported by a @ref kMvrEngineDidFinishReceivingNotification), the data they provide is no longer useful and may not reflect reality.
 */
@protocol MvrMiniIncoming <NSObject>

/** The type of the item being transferred. This starts out as nil (unknown), but becomes known before the transfer ends if it does not end with an error.
 
 You can use Key-Value Observing to observe changes to this key as the transfer progresses.
 */
- (NSString*) type;

/** The progress of this transfer as a floating-point number between 0.0 and 1.0, inclusive; or @ref kMvrMiniIndeterminateProgress if unknown. Note that a progress of 1.0 indicates that the transfer is nearing its end, not that it's finished.

 You can use Key-Value Observing to observe changes to this key as the transfer progresses.
 */
- (float) progress;

@end
