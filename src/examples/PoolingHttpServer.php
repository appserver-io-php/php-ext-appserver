<?php
class WebWorker extends Worker {
    public function __construct($requestHandler) {
        $this->requestHandler = $requestHandler;
    }
    protected $requestHandler;
}

class WebWork extends Collectable {
    public function __construct($client) {
        $this->client = $client;
    }
    public function run() {
        $client = $this->client;
        $this->worker->requestHandler->handle($client);
        $this->setGarbage();
    }
}

class RequestHandler extends Threaded {

    protected function handle($client) {
        // dummy read request
        $request = fread($client, 1024);
        // build up response
        $responseBody = "<html>" . "\r\n" .
            "<head>" . "\r\n" .
            "<title>Welcome!</title>" . "\r\n" .
            "</head>" . "\r\n" .
            "<body bgcolor=\"white\" text=\"black\">" . "\r\n" .
            "<center><h1>Welcome!</h1></center>" . "\r\n" .
            "</body>" . "\r\n" .
            "</html>" . "\r\n";
        $responseBody = str_replace(
            '{{server}}', SERVER_NAME, $responseBody
        );
        $responseHeaders =
            "HTTP/1.1 200 OK" . "\r\n" .
            "Server: {{server}}" . "\r\n" .
            "Date: {{date}}" . "\r\n" .
            "Content-Type: text/html" . "\r\n" .
            "Content-Length: {{content-length}}" . "\r\n" .
            "Connection: close" . "\r\n";
        $responseHeaders = str_replace(
            array('{{server}}', '{{date}}', '{{content-length}}'),
            array('Pooling/1.0', gmdate('D, d M Y H:i:s T'), strlen($responseBody)),
            $responseHeaders
        );
        fwrite($client, $responseHeaders . "\r\n" . $responseBody);
        fclose($client);
    }

}

$serverStreamSocketResource = stream_socket_server('tcp://127.0.0.1:8777',
    $errno, $errstr, STREAM_SERVER_BIND | STREAM_SERVER_LISTEN,
    stream_context_create(array('socket' => array('backlog' => 1024)))
);

$pool = new Pool(8, 'WebWorker', [new RequestHandler()]);


while (1) {
    if ($client = @stream_socket_accept($serverStreamSocketResource)) {
        $pool->submit(new WebWork($client));
    }
}

?>
