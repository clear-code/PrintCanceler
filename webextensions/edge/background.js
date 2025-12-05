'use strict';
const BROWSER = 'edge';
const SERVER_NAME = 'com.clear_code.print_canceler';

chrome.scripting.registerContentScripts([{
  id: 'print_canceler_main',
  matches: ['<all_urls>'],
  js: ['content-main.js'],
  allFrames: true,
  runAt: 'document_start',
  world: 'MAIN',
}]);

chrome.scripting.registerContentScripts([{
  id: 'print_canceler_isolated',
  matches: ['<all_urls>'],
  js: ['content-isolated.js'],
  allFrames: true,
  runAt: 'document_start',
  world: 'ISOLATED',
}]);


chrome.runtime.onMessage.addListener((message, _sender, _sendResponse) => {
  console.log('accept message.');
  switch (message?.type) {
    case 'deny': {
      const query = new String('B ' + BROWSER);
      chrome.runtime.sendNativeMessage(SERVER_NAME, query);
    }; break;

    case 'allow': {
      const query = new String('E ' + BROWSER);
      chrome.runtime.sendNativeMessage(SERVER_NAME, query);
    }; break;
  }
  return true;
});

console.log('service worker started.');