//
//  DMRecognizerAppDelegate.m
//  DMRecognizer
//
// Copyright 2010, Nuance Communications Inc. All rights reserved.
//
// Nuance Communications, Inc. provides this document without representation 
// or warranty of any kind. The information in this document is subject to 
// change without notice and does not represent a commitment by Nuance 
// Communications, Inc. The software and/or databases described in this 
// document are furnished under a license agreement and may be used or 
// copied only in accordance with the terms of such license agreement.  
// Without limiting the rights under copyright reserved herein, and except 
// as permitted by such license agreement, no part of this document may be 
// reproduced or transmitted in any form or by any means, including, without 
// limitation, electronic, mechanical, photocopying, recording, or otherwise, 
// or transferred to information storage and retrieval systems, without the 
// prior written permission of Nuance Communications, Inc.
// 
// Nuance, the Nuance logo, Nuance Recognizer, and Nuance Vocalizer are 
// trademarks or registered trademarks of Nuance Communications, Inc. or its 
// affiliates in the United States and/or other countries. All other 
// trademarks referenced herein are the property of their respective owners.
//

#import "DMRecognizerAppDelegate.h"
#import "DMRecognizerViewController.h"
#import "ScanViewController.h"

#import "RFduinoManager.h"
#import "RFduino.h"


@interface DMRecognizerAppDelegate()
{
    RFduinoManager *rfduinoManager;
    bool wasScanning;
}
@end

@implementation DMRecognizerAppDelegate

@synthesize window;
@synthesize viewController;


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {    
    
    // Override point for customization after app launch    
    [window setRootViewController:viewController];
    [window makeKeyAndVisible];
    
    rfduinoManager = RFduinoManager.sharedRFduinoManager;
    ScanViewController *tviewController = [[ScanViewController alloc] init];
    
    UINavigationController *navController = [[UINavigationController alloc] initWithRootViewController:tviewController];
    [self.window setRootViewController:navController];
    
    navController.navigationBar.tintColor = [UIColor colorWithRed:0.0 green:0.0 blue:0.0 alpha:1.0];
    
    self.window.backgroundColor = [UIColor whiteColor];
    [self.window makeKeyAndVisible];
    
    return YES;
}


- (void)applicationWillResignActive:(UIApplication *)application
{
    wasScanning = false;
    
    if (rfduinoManager.isScanning) {
        wasScanning = true;
        [rfduinoManager stopScan];
    }
    [viewController.voiceSearch cancel];
}
- (void)applicationDidBecomeActive:(UIApplication *)application
{
    NSLog(@"applicationDidBecomeActive");
    
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
    
    if (wasScanning) {
        [rfduinoManager startScan];
        wasScanning = false;
    }
}
-(void)setVCAndRFDuino:(RFduino *)duino{
    viewController.rfduino = duino;
    [window setRootViewController:viewController];
    [window makeKeyAndVisible];
    AVSpeechUtterance *utterance = [[AVSpeechUtterance alloc] initWithString:@"Tap the screen to set destination"];
    utterance.voice = [AVSpeechSynthesisVoice voiceWithLanguage:@"en-us"];
    [viewController.synthesizer speakUtterance:utterance];

}
//- (void)dealloc {
//    [viewController release];
//    [window release];
//    [super dealloc];
//}


@end
