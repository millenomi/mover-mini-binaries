//
//  MvrMiniItemAction.h
//  MoverMini
//
//  Created by ∞ on 09/08/10.
//  Copyright 2010 Emanuele Vulcano (Infinite Labs). All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MvrMiniItem.h"

typedef void (^MvrMiniItemActionBlock)(MvrMiniItem* item);

/**
 An item action is an action that can be performed on an item on the Mover table. For example, you may provide an "Open" action to move the item's contents to your application. The behavior of an item action can be given as a block, or be specified through a target/action pair. Actions are displayed on the Mover table as buttons when an item is selected by touching, and may also be used in other contexts.
 
 Each item has a title. This title can be literal, in which case is used as-is, or can contain format specifiers (such as <code>%(title)</code>) that are replaced with data extracted from the item the action could be invoked on. (Alternatively, you can override #titleForItem: to produce custom titles as you wish.)
 */
@interface MvrMiniItemAction : NSObject {}

// action is of the form -performActionOnItem:(MvrMiniItem*) i forAction:(MvrMiniItemAction*) a;
/**
 Creates an item action that, when performed, invokes the given action selector on the target.
 
 The action selector is of the form <code>-performActionOnItem:(MvrMiniItem*) i forAction:(MvrMiniItemAction*) a;</code>.
 */
- (id) initWithTitle:(NSString *)title replaceFormatSpecifiers:(BOOL) specs target:(id)target action:(SEL)action;

/**
 Creates an item action that, when performed, invokes the given block.
 */
- (id) initWithTitle:(NSString *)title replaceFormatSpecifiers:(BOOL) specs actionBlock:(MvrMiniItemActionBlock)actionBlock;

/**
 Creates an item action. This is the designated constructor for subclasses.
 */
- (id) initWithTitle:(NSString*) title replaceFormatSpecifiers:(BOOL) specs; // for subclasses

/**
 Convenience constructor for item actions that invoke an action on a target.
 */
+ (id) actionWithTitle:(NSString*) title replaceFormatSpecifiers:(BOOL) specs target:(id) target action:(SEL) action;
/**
 Convenience constructor for item actions that invoke a block.
 */
+ (id) actionWithTitle:(NSString*) title replaceFormatSpecifiers:(BOOL) specs actionBlock:(MvrMiniItemActionBlock) actionBlock;

/**
 The title that was passed to this class's constructor. This title may include format specifier if #replaceFormatSpecifiersInTitle is YES.
 */
@property(readonly, copy) NSString* title;

/**
 The target that will receive the action message. It may be nil in case a block is invoked instead.
 */
@property(readonly, assign) id target;

/**
 The action message that will be invoked. It may be nil in case a block is invoked instead.
 */
@property(readonly, assign) SEL action;

/**
 If YES, the default implementation of #titleForItem: will replace format specifiers from the #title.
 
 Format specifiers are of the form <code>%(…)</code>. The only format specifier this class currently has is <code>%(title)</code>, which is replaced with the item's title. See the #titleForItem: method for details.
 */
@property(readonly, assign) BOOL replaceFormatSpecifiersInTitle;

/**
 Returns whether this action is enabled. Disabled actions may be displayed, but will not be selectable by the user. See the #isAvailableForItem: method for details.
 */
@property(assign, getter=isEnabled) BOOL enabled;

/**
 Returns the title for the action given the specified item. The default implementation returns the value of the #title property, replacing format specifiers if this feature is enabled.
 */
- (NSString*) titleForItem:(MvrMiniItem*) i;

/**
 Returns whether the action is available to be performed on the specified item. The default implementation returns the value of the #enabled property.
 */
- (BOOL) isAvailableForItem:(MvrMiniItem*) i;

/**
 Performs the item action on the specified item. The default implementation invokes the action on the specified target, or invokes the block, depending on what default constructor was called. Subclasses that do not call either default constructor must override this method.
 */
- (void) performActionOnItem:(MvrMiniItem*) i;

@end
