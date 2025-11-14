Add-Type -AssemblyName System.Net

$listener = New-Object System.Net.HttpListener
$listener.Prefixes.Add("http://localhost:8080/")
$listener.Start()
Write-Host "HTTP server running: http://localhost:8080/"

while ($listener.IsListening) {
    try {
        $context = $listener.GetContext()
        $response = $context.Response
        $request = $context.Request

        # Disable cache in order to cause ERR_CACHE_MISS
        $response.Headers.Add("Cache-Control", "no-store, no-cache, must-revalidate")
        $response.Headers.Add("Pragma", "no-cache")
        $response.ContentType = "text/html"

        $html = @"
<!DOCTYPE html>
<html>
<head><title>PowerShell HTTP Server</title></head>
<body>
<h1>No cached page</h1>
<p>This page is not cached</p>
</body>
</html>
"@

        $buffer = [System.Text.Encoding]::UTF8.GetBytes($html)
        $response.ContentLength64 = $buffer.Length
        $response.OutputStream.Write($buffer, 0, $buffer.Length)
        $response.OutputStream.Close()
    } catch {
        Write-Warning "Error: $_"
    }
}
