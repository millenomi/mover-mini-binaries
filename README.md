# Mover Mini Pre-alpha

This is a pre-alpha version of the Mover Mini library. This library allows you to add some of the features of the [Mover application](http://infinite-labs.net/mover/) to your own app, allowing it to share and receive from other devices at the swipe of a finger.

## READ ME FIRST: Known issues.

This version of Mover Mini has the following known issues which will be _resolved in future releases_:

 * **THIS LIBRARY IS NOT READY FOR PRODUCTION.** It includes an expiration date of **October 1st, 2010**. Do not deploy apps that include this library to end users.

 * **Running on iPad is unsupported**. The application will crash on iPad. Do not attempt to deploy apps that include this library on iPad hardware.

 * **Only the portrait orientation is supported**. Showing the Mover table while the app has any part of its UI in landscape orientation, or changing the orientation while the Mover table is showing, will produce undefined behavior.

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
