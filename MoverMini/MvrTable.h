//
//  MvrTable.h
//  MoverMini
//
//  Created by ∞ on 03/08/10.
//  Copyright 2010 Emanuele Vulcano (Infinite Labs). All rights reserved.
//

#import <Foundation/Foundation.h>

@class MvrMiniItem, L0KVODispatcher, MvrSlide, MvrEngine;

/**
 This type defines the possible animation types for a MvrTable. See the MvrTable#animationStyle property for more information.
 */
typedef enum {
	/** This animation style will cause the Mover table to appear from a side of the screen and push the contents of the window aside. (The contents of the window will be partially visible near the table.)
	 
	 In the phone idiom (that is, when running on an iPhone app), the table appears at the bottom of the window and pushes the views up.
	 
	 Running the animation will cause the origins of all views in the window to be changed as part of the animation, and changed back as the animation undoes itself on hiding.
	 
	 This animation style cannot be undone cleanly if the orientation changes. It is the app's responsibility to prevent orientation changes while the table is showing with this animation style. If this cannot be avoided, you can use the Cover animation style instead.
	 
	 This is the default animation style.
	 */
	kMvrTableAnimationShift = 0,
	
	/** This animation style will cause the Mover table to appear as a view above all other views in the attached window. (The contents of the window will be partially visible above the table.)
	 
	 Unlike @ref kMvrTableAnimationShift, this animation style will not modify the origin of other views in the same window. */
	kMvrTableAnimationCover = 1,
} MvrTableAnimationStyle;


@protocol MvrTableDelegate;

/**
 A MvrTable instance manages the 'table', the main UI provided by Mover Mini. The table appears underneath all controls in a window, in a way reminiscent of modal view controllers; the table will appear if you request it with the #show or #showByAddingItem: methods, and may also automatically appear when certain events occur (for example, an item is received). 
 */
@interface MvrTable : NSObject {
@private
	UIStatusBarStyle statusBarStyle;
	
	NSMutableSet* displayedSlides;
	MvrSlide* selectedSlide;
	
	L0KVODispatcher* observer;
	
	NSMutableArray* barButtonItems;
	NSMutableArray* itemActions;
}

/**
 Returns YES if this is a supported configuration. Mover Mini only supports certain configurations. If the configuration is unsupported, you will not be able to produce or access MvrEngine and MvrTable instances (relevant access methods will return nil).
 
 Currently Mover Mini 1.0 is only supported on iPhone and iPod touch on iOS 4.0 and later.
 */
+ (BOOL) canUseMoverTable;

/**
 Returns YES if the table can be displayed in this configuration, or NO otherwise. Calling #show and #showByAddingItem: will only succeed in showing the table if this property returns YES (although items will still be added to the table silently anyway).
 
 The MvrTable objects will call the MvrTableDelegate#moverTableDidChangeCanShow: method whenever the value of this property has potentially changed.
 */
@property(readonly) BOOL canShowTable;

/**
 Creates a table attached to the given window. It will be displayed therein.
 */
- (id) initAttachedToWindow:(UIWindow *)w;

/**
 Returns the window this table is attached to.
 */
@property(readonly, assign) UIWindow* window;

/**
 Sets the animation style that will be used when the table is shown or hidden. See the @ref MvrTableAnimationStyle type for available animation styles. The default animation style is @ref kMvrTableAnimationShift.
 
 You can only set the style when the table is not shown onscreen. Setting the property while the table is onscreen produces undefined behavior.
 */
@property(assign, nonatomic) MvrTableAnimationStyle animationStyle;

/**
 Displays the table within the window this table is attached to, animating its entrance.
 
 If the table was already displayed, nothing will happen.
 */
- (void) show;

/**
 Hides the table if it's displayed, animating its disappearance.
 */
- (void) hide;

/**
 Adds an item to the table and displays it entering. If the table was already displayed, the item will be animated entering from the top edge of it. If the table was not displayed, the item will be animated entering as the table shows itself.
 
 This method adds the item to the known items set managed by @ref MvrEngine.
 
 Sometimes, the table may not be displayed (in which case #canShowTable returns NO). Calling this method in those situations will not show the table, but will still add the item.
 */
- (void) showByAddingItem:(MvrMiniItem*) item;

/**
 Sets and retrieves the actions can be performed on items on this table. Actions are shown as buttons on the table's bottom toolbar when a slide is touched. This array contains a number of MvrMiniItemAction objects, one for each action.
 
 The first action in this array, if any, is the 'default action' and is displayed more prominently than other actions. Additionally, the default action may be invoked with alternate means (for example, dragging the slide towards the app at the top side of the table, or by double-tapping).
 */
@property(copy, nonatomic) NSArray* itemActions;

/**
 The engine that powers this table. Since this version of Mover Mini only supports one running engine per application, this property is just a shorthand for retrieving the same instance returned by MvrEngine#defaultEngine.
 */
@property(readonly, nonatomic) MvrEngine* engine;

/**
 The table delegate can be used to fine-tune the behavior of the table. For more information on the delegate, see the MvrTableDelegate protocol.
 */
@property(assign) id <MvrTableDelegate> delegate;

@end

typedef enum {
	/** The table will be shown because the application explicitly invoked the MvrTable#showByAddingItem: method. */
	kMvrTableShowCauseProgrammatic = 0,
	
	/** The table will be shown because an item was received from the network. */
	kMvrTableShowCauseReceivedItem = 1,
} MvrTableShowCause;


/**
 A table delegate will be informed of events coming from a MvrTable instance. See the MvrTable#delegate property for more information.
 */
@protocol MvrTableDelegate <NSObject>
@optional

/**
 This method is called every time the table desires to display itself. This can happen due to various causes, both programmatically (as the result of your code calling the MvrTable#showByAddingItem: method) or because a new item has been received and the table desires to show it.
 
 You can return NO to prevent displaying the table for the item. This method is not called for all received items; the table may not bother calling it if it determines it cannot show itself anyway.
 */
- (BOOL) moverTable:(MvrTable*) table shouldShowForNewlyAddedItem:(MvrMiniItem*) item withCause:(MvrTableShowCause) cause;

/**
 Called when the table is about to be shown onscreen.
 */
- (void) moverTableWillShow:(MvrTable*) table;

/**
 Called when the table is about to be hidden.
 */
- (void) moverTableDidHide:(MvrTable*) table;

/**
 Called when the screen configuration has changed and the table has potentially changed its MvrTable#canShowTable property. Call that method to retrieve the new value of the property.
 
 Please note that the property might not have changed since the last time this delegate method was called.
 */
- (void) moverTableDidChangeCanShow:(MvrTable*) table;

@end
