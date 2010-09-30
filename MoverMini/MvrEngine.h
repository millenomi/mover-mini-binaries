//
//  MvrEngine.h
//  MoverMini
//
//  Created by ∞ on 14/07/10.
//  Copyright 2010 Emanuele Vulcano (Infinite Labs). All rights reserved.
//

#import <UIKit/UIKit.h>

@class L0Map;
@protocol MvrEndpoint;

/**
 Checks whether Mover Mini is currently available; it returns YES if it is, or NO if it is not.
 
 If this function returns NO, no Mover Mini function, method, constant or class is safe for use. Call this function before using any of the above. Caching the result of this function call is allowed (that is, the value will not change during the lifetime of the application).
 */
extern BOOL MvrMiniIsAvailable();


/**
 This notification is posted whenever the engine stops (as per the MvrEngine#stop method).
 */
extern NSString* const kMvrEngineWillStopNotification;

/** This notification is posted whenever the engine finds a new endpoint, that is, a remote device running a compatible version of Mover Mini or Mover. The new endpoint is the object at the @ref kMvrEngineEndpointKey key in the notification's info dictionary.
 
	Please note that an endpoint may be found multiple times by the engine. It's up to you to filter multiple appearances of the same endpoint in your code. (The MvrTable automatically does this for its own part when displaying them as arrows on the table.)
 */
extern NSString* const kMvrEngineDidFindEndpointNotification;


/** This notification is posted whenever the engine finds that an endpoint is no longer available. The disappeared endpoint is the object at the @ref kMvrEngineEndpointKey key in the notification's info dictionary. */
extern NSString* const kMvrEngineDidLoseEndpointNotification;

/** The object for this key in a @ref kMvrEngineDidFindEndpointNotification or @ref kMvrEngineDidLoseEndpointNotification's user info dictionary is an endpoint object (conforming to the MvrEndpoint protocol). */
extern NSString* const kMvrEngineEndpointKey;

/** This notification is posted whenever the engine begins receiving an item on your behalf.
 
 If an item is sent by another client whose type is not in MvrEngine#allowedItemTypes, the item is silently rejected; this notification is not sent in this case.
 
 The information about the incoming transfer is available through the object at the user info's @ref kMvrEngineIncomingKey.
 */
extern NSString* const kMvrEngineDidStartReceivingNotification;

/** The object for this key in a @ref kMvrEngineDidStartReceivingNotification or @ref kMvrEngineDidFinishReceivingNotification is a MvrMiniIncoming object describing an incoming transfer.
 */
extern NSString* const kMvrEngineIncomingKey;

/** This notification is posted whenever an engine has successfully received an allowed item on your behalf.
 
 The transfer has finished for the incoming transfer object at the user info's @ref kMvrEngineIncomingKey. If the transfer completed successfully, the received MvrMiniItem is found at the user info's @ref kMvrEngineItemKey; otherwise, this key will be unset.
 
 The notification is sent before the MvrEngine#knownItems set is altered.
 */
extern NSString* const kMvrEngineDidFinishReceivingNotification;

/** The object for this key in a kMvrEngineDidFinishReceivingNotification, kMvrEngineDidStartSendingNotification or kMvrEngineDidFinishSendingNotification is a MvrMiniItem representing the latest successfully received item. */
extern NSString* const kMvrEngineItemKey;


/** This notification is posted whenever an engine has started sending an item on your behalf. This happens whenever the user slides an item off the table to be sent.
 
 The item to be sent is available in the user info dictionary under the kMvrEngineItemKey. The endpoint is available under the kMvrEngineEndpointKey. A MvrMiniOutgoing object describing the transfer is available under the kMvrEngineOutgoingKey.
 */
extern NSString* const kMvrEngineDidStartSendingNotification;

/** This notification is posted whenever the engine has finished sending an item on your behalf.

 The item that was sent is available in the user info dictionary under the kMvrEngineItemKey. A MvrMiniOutgoing object describing the transfer is available under the kMvrEngineOutgoingKey. If an error occurred while sending, it's available in the user info dictionary under the kMvrEngineErrorKey.
 */
extern NSString* const kMvrEngineDidFinishSendingNotification;

/** The object for this key in a kMvrEngineDidFinishSendingNotification is the error that prevented the item from being sent. If no error occurred, this key is absent from the user info dictionary.
 */
extern NSString* const kMvrEngineErrorKey;

/** The object for this key in a @ref kMvrEngineDidStartSendingNotification or @ref kMvrEngineDidFinishReceivingNotification is a MvrMiniOutgoing object describing an outgoing transfer.
 */
extern NSString* const kMvrEngineOutgoingKey;


@class MvrModernWiFi, MvrScannerObserver, MvrMiniItem, L0UUID;

/**
 The engine coordinates Mover Mini activities, runs the Core so that discovery and transfers can happen, and produces endpoint (@ref MvrEndpoint) and item (@ref MvrMiniItem) objects for you.
 You rarely interact directly with the engine (and you cannot access the private Core it manages); instead, you usually interact with the higher-level @ref MvrTable instance attached to your app's window, which automatically displays items and endpoints and starts transfers if needed.
 */
@interface MvrEngine : NSObject {
	MvrModernWiFi* wifi;
	MvrScannerObserver* observer;
	L0UUID* identifier;
	
	NSMutableSet* knownItemsSet;
	
	BOOL registeredForShutdown, hasStoppedAfterEnteringBackground;
	
	NSSet* allowedItemTypes;
	L0Map* incomingsBeingChecked;
	NSMutableSet* incomingsNotYetChecked;
	
	NSUInteger retainCountForKnownItems;
	BOOL isAnythingGoingOn;
	
	NSMutableSet* miniItemsBeingSent;
}

/** Returns a reference to the Mover Mini resources bundle. */
+ (NSBundle*) moverMiniResourcesBundle;

/** Returns the default engine instance. This method may return nil if the engine is unavailable in the current configuration (see MvrTable#canUseMoverTable for more information). */
+ (MvrEngine*) defaultEngine;

/**
 Starts the engine. It will start listening and advertising this app on the local network, and keeping track of incoming items and endpoints. You only get notifications from this object if it's started.
 
 This method does nothing if the engine has already started. You may not need to call this method directly; showing a MvrTable in a window will automatically start the engine for you.
 */
- (void) start;

/**
 Stops the engine. It will undo the effects of the #start method. It will also try to reduce Mover Mini memory usage to a minimum, may clear the #knownItems property (which may cause temporary items to be deleted permanently), and may cause displayed tables to hide. A stopped engine may (but is not guaranteed to) cancel any ongoing transfers.
 
 Please note: a stopped object may stop sending you notifications. MvrMiniItem objects you retained remain valid, but endpoint objects no longer are after stopping.
 */
- (void) stop;

/**
 YES if the engine has started (via #start) but not stopped yet (via #stop). NO otherwise.
 */
@property(readonly, getter=isStarted) BOOL started;

/** The display name for this application. Used throughout the Mover Mini UI and transmitted over the network. Defaults to autodetection (usually by reading from the application bundle's Info.plist).

 If you want to change it, set it before starting the engine the first time. Subsequent changes are not guaranteed to stick. */
@property(copy) NSString* applicationDisplayName;

/** This set contains all the items (MvrMiniItem instances) the engine knows of. This includes:
 - Items you added to the table (for example via MvrTable's methods), and
 - Items that were received over the network.
 
 You can use Key-Value Observing to observe changes to this set.

 Think of this set as the set of all items that would get displayed when showing a Mover Mini table, because that's exactly what it is.

 This property's contents may be cleared when the #stop method is called, and may be modified from time to time while the engine is running. */
@property(readonly) NSSet* knownItems;

/** Adds an item to the known items set. It will appear on all tables.
 You don't typically call this method directly; instead, call the MvrTable#showByAddingItem: object to show a table to the user with the new item on it. That method will call this one as a side effect.
 */
- (void) addKnownItem:(MvrMiniItem*) i;

/** Removes all items from the known items set, clearing all tables and deleting all temporary items not retained elsewhere. */
- (void) removeAllKnownItems;


/** Sets and retrieves the allowed item types for this engine. You can only set allowed types while the engine is stopped.
 
 If this set is empty or nil, then items of all types are allowed. Otherwise, items whose types are not in this set will be silently rejected by the engine as they begin to arrive. By default, this property is set to nil. (For more information on item types, see MvrMiniItem#type.)
 
 Note that this changes the timing of arrival for MvrMiniItem objects; when this set is nonempty, you will not receive a @ref kMvrEngineDidStartReceivingNotification until the item has partially downloaded and is at the point where metadata has arrived. When it's empty, you will receive a @ref kMvrEngineDidStartReceivingNotification as soon as the item download starts.
 */
@property(nonatomic, copy) NSSet* allowedItemTypes;


/** Causes the engine to stop temporarily the automatic cleanup of items. If this method is NOT called, the engine will clean up the #knownItems property regularly, removing all known items and clearing all tables. If it is called, the temporary cleanup is prevented until a matching call to #releaseKnownItems.
 
 Note that calling #stop may clean up known items regardless of their retain status. Clients that sent this message must still call #releaseKnownItems before automatic cleanup resumes, but the cleanup induced by #stop is not affected by these calls.
 
 This method is also called at times by internal machinery to prevent items being lost during certain operations, notably while a table is showing or while a transfer is in progress.
 
 This method can be called multiple times, in which case an equal number of matching #releaseKnownItems calls is required to resume automatic cleanup.
*/
- (void) retainKnownItems;

/** Causes the engine to resume automatic cleanup of items. See #retainKnownItems for more information. */
- (void) releaseKnownItems;


/** Returns a set of all incoming transfers (as MvrMiniIncoming objects) from a given endpoint. */
- (NSSet*) incomingTransfersFrom:(id <MvrEndpoint>) dest;

/** Returns a set of all outgoing transfers (as MvrMiniOutgoing objects) going to a given endpoint. */
- (NSSet*) outgoingTransfersTo:(id <MvrEndpoint>) dest;


@end

