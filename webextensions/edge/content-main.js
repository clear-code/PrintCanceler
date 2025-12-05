const originalPrint = window.print.bind(window);
window.print = function ()
{
    console.log("overloaded window.print");
    window.postMessage({ type: 'FROM_PRINT_CANCELER_CONTENT_SCRIPT_MAIN' });
    if (originalPrint)
    {
        setTimeout(function() {
            originalPrint();
        }, 200);
    }
};
