//
//  UUIDGeneratorDelegate.h
//  UUIDGenerator
//
//  Created by ldx on 08-10-23.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface UUIDGeneratorDelegate : NSObject {
	IBOutlet	NSTextField*	uuidfield;
	IBOutlet	NSButton*		generate;
	IBOutlet	NSButton*		copytopasteboard;
	NSString*					uuid;
}

-(IBAction)buttonPressed:(id)sender;
-(IBAction)copyToPasteboard:(id)sender;
@end
