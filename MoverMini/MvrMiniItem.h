//
//  MvrMiniItem.h
//  MoverMini
//
//  Created by ∞ on 03/08/10.
//  Copyright 2010 Emanuele Vulcano (Infinite Labs). All rights reserved.
//

#import <Foundation/Foundation.h>

@class MvrItem;

/** The key in the MvrMiniItem#userInfo dictionary that corresponds to the item's title (returned by MvrMiniItem#title). Its string value is @"MvrTitle". */
extern NSString* const kMvrMiniItemTitleKey;

/** The key in the MvrMiniItem#userInfo dictionary that corresponds to the original file name, if any. Its string value is @"MvrOriginalFilename". */
extern NSString* const kMvrMiniItemOriginalFilenameKey;

/**
 An item represents a block of data and metadata that can be transferred by the Mover engine over the network. You construct one from a file or a NSData object, and then pass it to the MvrTable object to let the user see it (as a slide on the table) and pass it to another host.
 */
@interface MvrMiniItem : NSObject {
@private
	MvrItem* actualItem;
	id memoryWarningObserver;
}

/**
 Creates a new item with the contents of the specified file. If an error occurs, it will be returned by reference and this method will return nil.
 
 The file you create will not be moved or copied and must remain available at all times until this object is deallocated.
 
 @param file The full path to a file to produce an item from.
 @param title The title of the object. This is shown as a label on the item's slide on the Mover table and transmitted as part of the item's metadata.
 @param uti The UTI describing the type of the file's content. It can be a known or unknown UTI.
 @param meta The metadata attached to this item. See the #userInfo property for more information.
 @param error If an error occurs and needs to be returned, it will be returned by reference through this pointer. If you're not interested in the error, you may pass NULL to ignore this argument.
 */
+ itemWithContentsOfFile:(NSString*) file title:(NSString*) title type:(id) uti userInfo:(NSDictionary*) meta error:(NSError**) error;

/**
 Creates a new item with the contents of the NSData object you pass.
 
 The data may be saved to a file if you request this item's path through the #path property. See the #path and #temporary properties for more information.

 @param data The contents of the item.
 @param title The title of the object. This is shown as a label on the item's slide on the Mover table and transmitted as part of the item's metadata.
 @param uti The UTI describing the type of the file's content. It can be a known or unknown UTI.
 @param meta The metadata attached to this item. See the #userInfo property for more information.
 
 */
+ itemWithData:(NSData*) data title:(NSString*) title type:(id) uti userInfo:(NSDictionary*) meta;

/**
 Returns the title of the item. This is shown as a label on the item's slide on the Mover table and transmitted as part of the item's metadata. */
@property(readonly) NSString* title;

/**
 Returns an UTI for this item. (For more information on UTIs, see the Apple documentation on Uniform Type Identifiers.)
 You may create items with known or unknown UTIs, and receive items with known or unknown UTIs (a 'known' UTI is one that the <code>UTTypeCopyDeclaration()</code> function can find a declaration for). If a UTI is unknown, it's treated by Mover Mini as only conforming to <code>'public.data'</code>.
 
 It's up to you to handle UTIs regardless of their known status. */
@property(readonly) id type;

/**
 Returns all the metadata this object was created with. A metadata dictionary is composed of string keys associated to string values only.
 
 A couple of restrictions apply to the metadata you can add to an item. Keys with the following prefixes are forbidden and may be overwritten by the Mover engine if you use them:
 
 - All keys starting with "Payload-", which are reserved for use by the transport protocol.
 - All keys starting with "Mvr", which are reserved for use by the Mover engine.
 
 As a partial exception, certain Mover engine keys starting with "Mvr" have their own constants (for example, the key you retrieve through the @ref kMvrMiniItemOriginalFilenameKey constant). You are allowed to use those keys, and they won't be overwritten by the engine.
 
 You are able to use metadata keys with any other prefix for your own private use. Metadata is transferred and saved alongside the item when it's passed between Mover Mini clients and the Mover application, but does not remain saved if the item is sent to the Mover Connect desktop application, so take care if you accept items coming from the desktop client.
 */
@property(readonly) NSDictionary* userInfo;

/**
 Returns a path to a file containing the item's contents. Calling this path triggers the internal machinery of the item to save its contents if it hasn't saved to disk already, so do not call this method unless absolutely necessary.
 
 This path may be the path you constructed the item with (if you did so by calling #itemWithContentsOfFile:title:type:userInfo:error:). Otherwise, it may be a path to a temporary location on disk, unless you have made the item persistent (by calling #makePersistentByMovingToPath:error:). The path is guaranteed to exist for as long as this object does.
 */
@property(readonly) NSString* path;

/**
 Returns whether the item is temporary (YES) or persistent (NO). A temporary item is created when data is received over the network; in this case, the data will be destroyed when the item object is deallocated. (This includes deleting any file whose path may be returned by the #path property). On the other hand a persistent object will not modify the file returned by the #path property and the file will keep existing past deallocation; a persistent item will never modify the data on disk, but may refer to it for reading only.
 
 You may be able to create temporary items in some cases, for example when using the #itemWithData:title:type:userInfo: method; additionally, items received from the network that you haven't saved explicitly are always temporary. You can make a temporary item persistent by moving the data to a known location you control with the #makePersistentByMovingToPath:error: method.
 */
@property(readonly, getter=isTemporary) BOOL temporary;

/**
 If this item is temporary, this method will make it persistent by saving its contents to a known path. Once saved this way, the item's contents will not be lost past the item being deallocated.
 
 This method will destroy any previous backing storage on disk, if any; the file path you pass must remain existing and available at all times while this object exists. Invoking this method on a persistent object will produce an error.
 */
- (BOOL) makePersistentByMovingToPath:(NSString*) p error:(NSError**) e;

@end
