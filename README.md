# Mover Mini Pre-alpha

This is a pre-alpha version of the Mover Mini library. This library allows you to add some of the features of the [Mover application](http://infinite-labs.net/mover/) to your own app, allowing it to share and receive from other devices at the swipe of a finger.

Following this section are update news. Look below them for a quick start on the use of this library.

# Update: Sep 30th, 2010

## The TL;DR

This update resolves some known issues and extends expiration to October 15th, 2010.

You can now use this library in apps that run on iPad. It will not work (constructor methods return `nil`), but it will no longer raise exceptions.

You can now use this library in apps that go landscape. Change the animation style of the table to `Cover` if the app may go landscape while the table is onscreen.

## The long version

You can now deploy this library in iPad applications and the library can behave correctly when the orientation changes, provided certain steps are taken.

* It is no longer necessary to check for the idiom before trying to create `MvrTable` and `MvrEngine` objects. Any attempt to access or create instances of these objects will return `nil` instead if the environment cannot support Mover Mini (for example, on iPad). You can check whether creating instances will work by invoking the `[MvrTable canUseMoverTable]` class function.

* The `Cover` animation style now supports orientation changes. Rotating the device will cause the `Cover` table to hide. The `Slide` animation style, which is the default, cannot recover correctly from an orientation change and will throw an exception instead if forced into an unrecoverable situation.

	The `Slide` animation is more aesthetically pleasant, all in all, and thus remains the default. If you use it you must guarantee that the application will never change its orientation while the table is onscreen. If you cannot guarantee it, you can use the `Cover` style instead now. (The table is only displayable in a portrait orientation, so landscape-only apps are not supported for now.)

* The `canShowTable` property has been added. If the orientation is not right, it will return `NO` and the `show` and `showByAddingItem:` methods will not display the table (although the item will still be added, if given). The table now posts `kMvrTableDidChangeCanShowNotification` to the default notification center when the value of this property has potentially changed. (It may also hide the table at this time, as above.)

The following known issues have changed, but remain:

* **THIS LIBRARY IS NOT READY FOR PRODUCTION.** It will raise an exception if used past its expiration date of **October 15th, 2010**. Do not deploy apps that include this library to end users.

* **Only iOS 4.0 and later is supported.** Do not add this library to an application that may run on iOS 3.x; both the base SDK and deployment target setting must be 4.0 or later. (This library has been built against the 4.1 base SDK.)


This version of Mover Mini has the following important notes:

 * The default animation style for the table (`Shift`) will manipulate views in your window to produce a "push up" effect. If you use this animation style, you must not modify the origin, add, or remove any direct subview of the window while the Mover table is displayed in it. If you need to modify the origin of, or remove, direct subviews of the window, you can use the alternative `Cover` animation style, which does not modify those views' origins.

 * Adding subviews to a window while the Mover table is displayed in it will not be supported.

## Adding it to your project.

To add Mover Mini to your application, you need to add the files in the "MoverMini" folder to your target. To do so:

 * Drag the "MoverMini" folder itself (not its contents) to the `Other Sources` group of your project.
 * In the add options dialog, make sure that:
	* The **'Copy Items…'** checkbox is checked;
	* The **'Recursively create groups…'** option is selected.
	* The checkbox on the right of your app target's entry in the list is checked (so that new items are added to the target).
 * Open your target's settings by choosing **Project > Edit Active Target…** from the menu.
 * In the **Build** tab, in the lower right corner, make sure the **Based On** combo box is set to "MoverMini". (Alternatively, copy and adapt the settings in the MoverMini.xcconfig to your target.)

## Using it.

Here are a few pointers to get started. For more detailed information, see the documentation in the headers or in the included "Docs" folder.

### How to Send Something through Mover:

To send something with Mover Mini, you add it to the table and let Mover handle the rest.

	// let's say it's a PNG image:
	#import <MoverMini/MoverMini.h>
	#import <MobileCoreServices/MobileCoreServices.h>

	// …
	
	MvrMiniItem* item = [MvrMiniItem itemWithContentsOfFile:pngFileToSend type:(id) kUTTypePNG userInfo:nil];
	[table showByAddingItem:item];
	
	// the user can then slide the picture with a finger to send it

### How to Receive Something:

As long as the Mover Mini engine is running, it will automatically receive stuff for you in the background. The table appears automatically as soon as an item is received.

To get that received stuff from the table, you need to add an action — the user will see actions when touching a received slide. For example, an "Open" action, or distinct "Mail" and "Tweet" actions. The first action you set in the actions array is displayed more prominently (it's the "default action").

Adding actions is simple:

	#import <MoverMini/MoverMini.h>

	MvrMiniItemAction* openAction = [MvrMiniItemAction
		actionWithTitle:@"Open %(title)"
		replaceFormatSpecifiers:YES // %(title) will become the title of the slide
		actionBlock:^(MvrMiniItem* receivedItem) {
			// Do cool stuff with the received item
		}];

	table.itemActions = [NSArray arrayWithObject:openAction];

To save a MvrMiniItem's contents to a file once you have it:

	NSError* e;
	if ([item makePersistentByMovingToPath:yourSavePath error:&e]) {
		
		// yay!
		
	} else { /* error */ }

### How to Clean Up after Mover Mini
It's automatic! The table is regularly cleared; once it happens, all temporary stuff will be cleaned up for you. Additionally, all temporary stuff is also cleaned up whenever the engine stops and whenever the application is quit cleanly.

If you want to explicitly clean up, use this:

	[table.engine removeAllKnownItems]; // clears the table!
	
Or you might want the opposite — to _not_ clean up for a while. Use this:

	[table.engine retainKnownItems];
	
	// do your (time-consuming) stuff.
	
	[table.engine releaseKnownItems];
	
Mover Mini is smart enough to do the above in certain situations automatically — it will never clean up while the user is using Mini (for example, when the table is open, or when a transfer is in progress).

### How to Control when Mover Mini Transfers are Available

To make Mini available, start the engine:

	[table.engine start];

As long as it's running, it will automatically receive stuff over the network. The engine is also started automatically whenever the table is shown. To turn it off, stop it:

	[table.engine stop];

This will hide the table, if showing.

### How to Control what Arrives

You can specify that you want to limit the Mover engine to only accept certain _types_ of items. (Each item is tagged with a type, an UTI string.)

For example, to set only PNG and JPEG images as allowed, do:

	#import <MobileCoreServices/MobileCoreServices.h>
	
	// ... code ...
	
	table.engine.allowedItemTypes = [NSSet setWithObjects:(id) kUTTypePNG, (id) kUTTypeJPEG, nil];
	
This will silently ignore all other kinds of items. Reminder: you need to set this before the engine starts!

### How to Show Icons on the Table

To specify how stuff looks like on the table, you need to register an "item displayer". There's one built-in for UIKit-decodable images; just run this line before you create the table:

	[MvrMiniImageDisplayer addDisplayer];
	
You can make displayers for your own items by making a `MvrMiniItemDisplayer` subclass, overriding the right methods (see the docs!), then calling `+addDisplayer` on it before you create your table the first time.
