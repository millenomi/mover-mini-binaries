//
//  MvrMiniImageDisplayer.h
//  MoverMini
//
//  Created by ∞ on 14/08/10.
//  Copyright 2010 Emanuele Vulcano (Infinite Labs). All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MvrMiniItemDisplayer.h"

/**
 This displayer shows images on the table as thumbnails; this is similar to how images are shown in the Mover application.
 
 This displayer is available as part of Mover Mini but is not registered by default. If you want to display images this way, you can register an instance of this class by calling its #addDisplayer method.
 */
@interface MvrMiniImageDisplayer : MvrMiniItemDisplayer {}

/**
 Adds an instance of the image displayer in the registered displayers set, as per MvrMiniItemDisplayer#addDisplayer.
 
 Calling this method is the preferred way of registering a MvrMiniImageDisplayer.
 */
+ (void) addDisplayer;

/**
 Returns the types this displayer can handle. This property has been overridden and is guaranteed to not produce any exceptions when accessed.
 
 Note however that it is not guaranteed that every item of this type is displayable through this instance (as reported by #canDisplayItem:).
 */
@property(readonly) NSSet* displayableItemTypes;

@end
