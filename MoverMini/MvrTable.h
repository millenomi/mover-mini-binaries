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
 This type defines the possible animation types for a MvrTable. See the MvrTable#animationKind property for more information.
 */
typedef enum {
	/** This animation style will cause the Mover table to appear from a side of the screen and push the contents of the window aside. (The contents of the window will be partially visible near the table.)
	 
	 In the phone idiom (that is, when running on an iPhone app), the table appears at the bottom of the window and pushes the views app.
	 
	 Running the animation will cause the origins of all views in the window to be changed as part of the animation, and changed back as the animation undoes itself on hiding.
	 
	 This is the default animation style.
	 */
	kMvrTableAnimationShift = 0,
	
	/** This animation style will cause the Mover table to appear as a view above all other views in the attached window. (The contents of the window will be partially visible above the table.)
	 
	 Unlike @ref kMvrTableAnimationShiftUp, this animation style will not modify the origin of other views in the same window. */
	kMvrTableAnimationCover = 1,
} MvrTableAnimationStyle;

@protocol MvrTableDelegate;

/**
 A MvrTable instance manages the 'table', the main UI provided by Mover Mini. The table appears underneath all controls in a window, in a way reminiscent of modal view controllers; the table will appear if you request it with the #show or #showByAddingItem: methods, and may also automatically appear when certain events occur (for example, an item is received). 
 */
@interface MvrTable : NSObject {
	UIStatusBarStyle statusBarStyle;
	
	NSMutableSet* displayedSlides;
	MvrSlide* selectedSlide;
	
	L0KVODispatcher* observer;
	
	NSMutableArray* barButtonItems;
	NSMutableArray* itemActions;
}

/**
 Creates a table attached to the given window. It will be displayed therein.
 */
- (id) initAttachedToWindow:(UIWindow *)w;

/**
 Returns the window this table is attached to.
 */
@property(readonly, assign) UIWindow* window;

/**
 Sets the animation kind that will be used when the table is shown or hidden. See the @ref MvrTableAnimationKind type for available animation kinds. The default animation style is @ref kMvrTableAnimationShiftUp.
 
 You can only set the kind when the table is not shown onscreen. Setting the property while the table is onscreen can cause arbitrary behavior to happen.
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

@protocol MvrTableDelegate <NSObject>
@optional

- (BOOL) moverTable:(MvrTable*) table shouldShowForNewlyAddedItem:(MvrMiniItem*) item withCause:(MvrTableShowCause) cause;

- (void) moverTableWillShow:(MvrTable*) table;
- (void) moverTableDidHide:(MvrTable*) table;

@end
