#pragma once

#include "ofMain.h"

#include "ofxLibwebsockets.h"

#define NUM_MESSAGES 20 // how many past messages we want to keep
class Ball : public ofVec2f
{

public:
    ofVec2f v;
        ofColor c;
    Ball(ofColor c , int x ,int y)
    {
        this->c = c;
        this->x = x;
        this->y = y;
        
    }
    
    void draw()
    {
        if(x<0)
        {
            v.x*=-1;
        }
        else if(x>ofGetWidth())
        {
            v.x*=-1;
        }
        if(y<0)
        {
            v.y*=-1;
        }
        else if(y>ofGetHeight())
        {
            v.y*=-1;
        }

        v*=0.99;
        x+=v.x;
        y+=v.y;
        
        ofPushStyle();
        ofSetColor(c);
        ofCircle(x, y, 10);
        ofPopStyle();
    }
};
class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
        ofxLibwebsockets::Server server;
    
        //queue of rec'd messages
        ofTrueTypeFont font;
        vector<string> messages;
        // websocket methods
        void onConnect( ofxLibwebsockets::Event& args );
        void onOpen( ofxLibwebsockets::Event& args );
        void onClose( ofxLibwebsockets::Event& args );
        void onIdle( ofxLibwebsockets::Event& args );
        void onMessage( ofxLibwebsockets::Event& args );
        void onBroadcast( ofxLibwebsockets::Event& args );
    
    map<string, Ball*> balls;

};
