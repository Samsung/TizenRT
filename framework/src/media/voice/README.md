# SpeechDetector
SpeechDetector would be used for Keyword Detection and EndPoint Detection on a voice solution.
Supports Hardware Detecting(If supported by H/W) and Software Detecting.

### Features
  - Keyword Detection Delegate (Such as "Hi, Bixby") : Synchronous Call
  - EndPoint Detection (When the saying is ended) : Callback Function(Delegate)

### How to use in a service?
```sh
void MyClass::StartVoice() {
    SpeechDetector *speechDetector = SpeechDetector::instance();
    speechDetector->initKeywordDetect(16000, 1); // SampleRate and Channels
    speechDetector->initEndPointDetect(16000, 1);
    
    speechDetector->setEndPointDetectedDelegate(std::bind(MyClass::OnEPD, this));
    if (speechDetector->startKeywordDetect() == true) {
        /*
        * Keyword is detected!
        * Start Recording!
        */
    }
}

void MyClass::OnEPD() {
    /*
    * Stop Recording!
    */
}
```
