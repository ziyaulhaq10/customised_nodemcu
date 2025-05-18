// HTML form page
const char uploadPage[] PROGMEM = R"rawliteral(
    <!DOCTYPE html>
    <html>
      <head><title>Upload JSON</title></head>
      <body>
        <h2>Upload JSON</h2>
        <form action="/upload" method="post">
          <textarea name="json" rows="10" cols="50"></textarea><br>
          <input type="submit" value="Upload">
        </form>
        <p><a href="/">Back</a> | <a href="/get">View Saved JSON</a></p>
      </body>
    </html>
    )rawliteral";


