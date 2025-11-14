chrome.scripting.registerContentScripts([{
  id: 'print_canceler_main',
  matches: ['<all_urls>'],
  js: ['content-main.js'],
  allFrames: true,
  runAt: 'document_start',
  world: "MAIN",
}]);

chrome.scripting.registerContentScripts([{
  id: 'print_canceler_isolated',
  matches: ['<all_urls>'],
  js: ['content-isolated.js'],
  allFrames: true,
  runAt: 'document_start',
  world: "ISOLATED",
}]);


chrome.runtime.onMessage.addListener((message, sender, sendResponse) => {
  console.log("accept message.");
  if (message === 'cancel') {
    sendResponse(user);
  }
  return true;
});

console.log("service worker started.");