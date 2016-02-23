//
//  DMRecognizerViewController.h
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

#import <UIKit/UIKit.h>
#import <SpeechKit/SpeechKit.h>
#import <AVFoundation/AVFoundation.h>
#import <CoreLocation/CoreLocation.h>
#import "RFduino.h"
#import <MapKit/MapKit.h>

@class MKMapView;
@interface DMRecognizerViewController : UIViewController <SpeechKitDelegate, SKRecognizerDelegate, UITextFieldDelegate, CLLocationManagerDelegate, RFduinoDelegate, MKMapViewDelegate> {
    IBOutlet UIButton* recordButton;
    IBOutlet UITextField* searchBox;
    IBOutlet UITextField* serverBox;
    IBOutlet UITextField* portBox;
    IBOutlet UITextView* alternativesDisplay;
    IBOutlet UIView* vuMeter;
    IBOutlet UISegmentedControl* recognitionType;
    IBOutlet UISegmentedControl* languageType;
    IBOutlet UITextField* curLocBox;
    IBOutlet UILabel *directions;


    SKRecognizer* voiceSearch;
    enum {
        TS_IDLE,
        TS_INITIAL,
        TS_RECORDING,
        TS_PROCESSING,
    } transactionState;
}
@property(nonatomic, retain) CLLocationManager *locationManager;
@property(nonatomic,retain) IBOutlet UIButton* recordButton;
@property(nonatomic,retain) IBOutlet UITextField* searchBox;
@property(nonatomic,retain) IBOutlet UITextField* curLocBox;

@property(nonatomic,retain) IBOutlet UITextField* serverBox;
@property(nonatomic,retain) IBOutlet UITextField* portBox;
@property(nonatomic,retain) IBOutlet UITextView* alternativesDisplay;
@property(nonatomic,retain) IBOutlet UIView* vuMeter;
@property(nonatomic,retain) IBOutlet UILabel* directions;

@property(readonly)         SKRecognizer* voiceSearch;
@property (strong, nonatomic) AVSpeechSynthesizer *synthesizer;

@property (nonatomic, retain) CLGeocoder *geocoder;
@property (nonatomic, retain) NSString *currentAddress;
@property (atomic, retain) CLLocation *currentLocation;

@property float curLocLat;
@property float curLocLon;

@property(strong, nonatomic) RFduino *rfduino;


@property(strong, nonatomic) CLPlacemark *placemark;
@property int simulatedPointCount;

@property(strong, nonatomic) MKMapView *mapView;
@property(strong, nonatomic) MKPolyline *prevPolyline;

@property(strong, nonatomic) NSMutableArray *angles;
@property(strong, nonatomic) MKRoute *currentRoute;
@property(strong, nonatomic) MKCircle *simulatedPosition;

@property int curPointIndex;
@property BOOL isDemo;
@property BOOL isNavigating;
- (IBAction)recordButtonAction: (id)sender;
- (IBAction)serverUpdateButtonAction: (id)sender;

@end

