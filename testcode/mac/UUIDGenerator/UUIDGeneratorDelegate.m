//
//  UUIDGeneratorDelegate.m
//  UUIDGenerator
//
//  Created by ldx on 08-10-23.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "UUIDGeneratorDelegate.h"


@implementation UUIDGeneratorDelegate

-(void)applicationDidFinishLaunching:(NSNotification*)note
{
	uuid = nil;
}
-(IBAction)buttonPressed:(id)sender
{
	if(uuid)
		[uuid release];
	CFUUIDRef idref = CFUUIDCreate(NULL);
	CFStringRef sref = CFUUIDCreateString(NULL, idref);
	[uuidfield setTitleWithMnemonic:sref];
	uuid = sref;

}

-(IBAction)copyToPasteboard:(id)sender
{	
	NSLog(@"uuid is %@", uuid);
	if(!uuid)
		return;
	NSPasteboard *pb = [NSPasteboard generalPasteboard];
	NSArray *types = [NSArray arrayWithObjects:NSStringPboardType, nil];
	[pb declareTypes:types owner:nil];
	[pb setString:uuid forType:NSStringPboardType];
}
@end
