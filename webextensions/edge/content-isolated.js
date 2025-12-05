window.addEventListener('beforeprint', _event => {
  console.log('Before print');
  console.log('window.isPrintEnabled', window.isPrintEnabled || false);
  const isPrintEnabled = window.isPrintEnabled;
  window.isPrintEnabled = false;
  if (!isPrintEnabled) {
    chrome.runtime.sendMessage({ type: 'cancel' }, _response => {
    });
  }
});

window.addEventListener('message', event => {
  if (event.source !== window) return;
  if (event.data.type && event.data.type === 'FROM_PRINT_CANCELER_CONTENT_SCRIPT_MAIN') {
    console.log('receive message from window.print');
    window.isPrintEnabled = true;
  }
});