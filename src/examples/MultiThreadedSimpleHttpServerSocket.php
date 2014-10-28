<?php

/**
 * Class ThreadWorker
 */
class ThreadWorker extends Thread
{
    /**
     * @param $serverSocketResource The server socket resource
     */
    public function __construct($serverSocketResource)
    {
        $this->serverSocketResource = $serverSocketResource;
    }

    /**
     * thread routine
     */
    public function run()
    {
        while ($clientSocketResource = socket_accept($this->serverSocketResource)) {
            $request = socket_read($clientSocketResource, 2048);

            // build up response
            $responseBody = "<html>" . "\r\n" .
                "<head>" . "\r\n" .
                "<title>Welcome to {{server}}!</title>" . "\r\n" .
                "</head>" . "\r\n" .
                "<body bgcolor=\"white\" text=\"black\">" . "\r\n" .
                "<center><h1>Welcome to {{server}}!</h1></center>" . "\r\n" .
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
                array('{{server}}','{{date}}','{{content-length}}'),
                array(SERVER_NAME, gmdate('D, d M Y H:i:s T'), strlen($responseBody)),
                $responseHeaders
            );
            $response = $responseHeaders . "\r\n" . $responseBody . "\r\n";
            socket_write($clientSocketResource, $response, strlen($response));
            socket_close($clientSocketResource);
        }
    }
}

/**
 * Class MultithreadedServer
 */
class MultithreadedServer extends Thread
{
    public function __construct($localSocket, $workerCount, $serverName)
    {
        define('SERVER_NAME', $serverName);

        $flags = STREAM_SERVER_BIND | STREAM_SERVER_LISTEN;
        // set socket backlog to 1024 for perform many concurrent connections
        $opts = array(
            'socket' => array(
                'tcp_nodelay' => true,
                'backlog' => 1024,
            )
        );
        // init socket
        $serverSocketResource = socket_create(AF_INET, SOCK_STREAM, 0);
        // bind the socket to an address/port
        socket_set_option($serverSocketResource, SOL_SOCKET, SO_REUSEADDR, 1);
        socket_bind($serverSocketResource, '127.0.0.1', '8999');
        // start listening for connections
        socket_listen($serverSocketResource, 65000);
        // set socket options
        socket_set_block($serverSocketResource);

        $this->workerCount = $workerCount;
        $this->serverSocketResource = $serverSocketResource;
    }

    public function run()
    {
        $workers = [];
        while (count($workers) < $this->workerCount) {
            $worker = new ThreadWorker($this->serverSocketResource);
            $worker->start();
            $workers[] = $worker;
        }
    }
}

$server = new MultithreadedServer('127.0.0.1:8999', 64, 'BenchmarkSimpleHttpServer/1.0');
$server->start();
