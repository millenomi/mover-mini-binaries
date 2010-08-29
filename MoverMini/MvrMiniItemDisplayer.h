//
//  MvrMiniItemDisplayer.h
//  MoverMini
//
//  Created by ∞ on 14/08/10.
//  Copyright 2010 Emanuele Vulcano (Infinite Labs). All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MvrMiniItem.h"

/** An item displayer produces information that a table (a MvrTable instance) uses to decorate that item's visual representation (the slide). For example, it produces the title and icon that are shown on the slide.
 
 You can create your own displayers for your own types. To do so, create a subclass and override (at a minimum) #displayableItemTypes and #imageOfSize:forItem:, though this assumes that you'll use default implementations for all other methods. Check up on the documentation for these methods to see what to override for more advanced functionality.
 
 Once you have created your own displayer class, you have to register an instance of it using the #addDisplayer: method (or the convenience #addDisplayer method, which creates one instance for you) before table instances will be able to use it.
 
 There is a default item displayer that produces a generic ("package") icon, which is returned for items for which no displayer is registered. Mover Mini also provides a UIKit-powered displayer for images which is not set up by default, but can be used if you want to transfer images; see the @ref MvrMiniImageDisplayer class for more information.
 */
@interface MvrMiniItemDisplayer : NSObject {}

/**
 Returns a displayer for the given item. If there are multiple displayers that can display the specified item, which one is returned exactly is undefined.
 
 If no displayer is available for the item, a generic displayer of a private class is returned instead.
 */
+ displayerForItem:(MvrMiniItem*) item;

/** Returns all registered displayer instances. This set does not include the generic displayer. */
+ (NSSet*) allDisplayers;

/** Adds the given displayer instances to the set of all registered displayers. Once added, a displayer will be available for all MvrTable instances to use. */
+ (void) addDisplayer:(MvrMiniItemDisplayer*) displayer;
/** Removes the given displayer from the set of all registered displayers. See #addDisplayer: for more information on registering. */
+ (void) removeDisplayer:(MvrMiniItemDisplayer*) displayer;

/** This is a convenience method that produces a displayer (calling -init as the constructor), then adds it to the set of all registered displayers. This is the recommended way to add a displayer. */
+ (void) addDisplayer;

/**
 Returns YES if this displayer can handle the specified item, NO otherwise. The default implementation of this method returns YES if the item's type is one of this displayer's displayable types (returned by #displayableItemTypes), NO otherwise; this implementation can be entirely overridden if needed.
 
 Note that #displayableItemTypes is an abstract method; therefore, letting this method's default implementation be called without overriding that one is a programmer error.
 */
- (BOOL) canDisplayItem:(MvrMiniItem*) item;

/**
 Returns the title for the specified item. The default implementation returns the MvrMiniItem#title, but this may be overridden if need be.
 */
- (NSString*) titleForItem:(MvrMiniItem*) item; // defaults to returning item.title.

// abstract

/**
 Returns a set of item types this instance can display. (For more information on item types, see MvrMiniItem#type.) This is used by the default implementation of #canDisplayItem:.
 
 The default implementation of this property is abstract and will cause an exception to be thrown when the property is accessed. You must override this method (or #canDisplayItem:) before your displayer can function properly. 
 */
@property(readonly) NSSet* displayableItemTypes;

/**
 Returns an image that represents the displayed item. The image should not exceed the given size; it can be larger, but it will be scaled down when displayed.
 
 The default implementation of this method is abstract and will cause an exception to be thrown when called. You must override this method before your displayer can function properly.
 */
- (UIImage*) imageOfSize:(CGSize) s forItem:(MvrMiniItem*) item;

@end
