#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    // setup a server with default options on port 9092
    // - pass in true after port to set up with SSL
    //bool connected = server.setup( 9092 );
    
    // Uncomment this to set up a server with a protocol
    // Right now, clients created via libwebsockets that are connecting to servers
    // made via libwebsockets seem to want a protocol. Hopefully this gets fixed, 
    // but until now you have to do something like this:
    
    ofxLibwebsockets::ServerOptions options = ofxLibwebsockets::defaultServerOptions();
    options.port = 9093;
//    options.protocol = "of-protocol";
//    options.bBinaryProtocol = true;
    
    bool connected = server.setup( options );
    
    // this adds your app as a listener for the server
    server.addListener(this);
    
    // setup message queue
    
    font.loadFont("myriad.ttf", 20);
    messages.push_back("WebSocket server setup at "+ofToString( server.getPort() ) + ( server.usingSSL() ? " with SSL" : " without SSL") );
    
    ofBackground(0);
    ofSetFrameRate(60);

}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){
    std::map<string,Ball*>::iterator it;
    for (it=balls.begin(); it!=balls.end(); ++it)
    {
        Ball* ball = (it->second);
        ball->draw();
    }
    
    int x = font.getSize();
    int y = font.getSize()*2;
    ofSetColor(255);
    for (int i = messages.size() -1; i >= 0; i-- ){
        //font.drawString( messages[i], x, y );
        y += font.stringHeight( messages[i] ) + font.getSize();
    }
    

}

//--------------------------------------------------------------
void testApp::onConnect( ofxLibwebsockets::Event& args ){
    cout<<"on connected"<<endl;
}

//--------------------------------------------------------------
void testApp::onOpen( ofxLibwebsockets::Event& args ){
    cout<<"new connection open"<<endl;
    messages.push_back("New connection from " + args.conn.getClientIP() );
    
    // send the latest image if there is one!
//    if ( currentImage.bAllocated() ){
//        args.conn.send( ofToString(currentImage.width) +":"+ ofToString( currentImage.height ) +":"+ ofToString( currentImage.type ) );
//        args.conn.sendBinary( currentImage );
//        
//    }
    balls.insert(std::pair<string ,Ball*>(args.conn.getClientIP(),new Ball(
                                                                          ofColor(ofRandom(255),
                                                                                  ofRandom(255)
                                                                                  ,ofRandom(255)),
                                                                          ofRandom(ofGetWidth()),
                                                                          ofRandom(ofGetHeight()))));


}

//--------------------------------------------------------------
void testApp::onClose( ofxLibwebsockets::Event& args ){
        balls.erase ( args.conn.getClientIP() );
    cout<<"on close"<< " " << args.conn.getClientIP() <<endl;
    messages.push_back("Connection closed");


}

//--------------------------------------------------------------
void testApp::onIdle( ofxLibwebsockets::Event& args ){
    cout<<"on idle"<<endl;
}

//--------------------------------------------------------------
void testApp::onMessage( ofxLibwebsockets::Event& args ){
    cout<<"got message "<<args.message<<endl;
    
    // trace out string messages or JSON messages!
    // args.json is null if badly formed or just not JOSN
    if ( !args.json.isNull() ){
        messages.push_back("New message: " + args.json.toStyledString() + " from " + args.conn.getClientName() );
        
        Json::Value v = args.json.get("vec", 0);
        if(v.size()==1)
        {
            int i = 0;
            Ball* ball = balls[args.conn.getClientIP()];
            if(ball!=NULL)
            {

                ball->v.set(v.get(i,0).get("x",0).asFloat(),v.get(i, 0).get("y",0).asFloat());

            }
        }
        Json::Value c = args.json.get("color", 0);
        if(c.size()==1)
        {
            int i = 0;
            Ball* ball = balls[args.conn.getClientIP()];
            if(ball!=NULL)
            {
                
                ball->c.set(c.get(i,0).get("r",0).asInt(),
                            c.get(i, 0).get("g",0).asInt(),
                            c.get(i, 0).get("b",0).asInt());
                
            }
        }
        
    } else {
        messages.push_back("New message: " + args.message + " from " + args.conn.getClientName() );
    }
    
    //if (messages.size() > NUM_MESSAGES) messages.erase( messages.begin() );
    
    // echo server = send message right back!
    args.conn.send( args.message );
}

//--------------------------------------------------------------
void testApp::onBroadcast( ofxLibwebsockets::Event& args ){
    cout<<"got broadcast "<<args.message<<endl;    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
    for (int i=0; i<dragInfo.files.size(); i++){
        string file = dragInfo.files[i];
        ofFile f(file); 
        string ex = f.getExtension();
        std::transform(ex.begin(), ex.end(),ex.begin(), ::toupper);
        
    }
}