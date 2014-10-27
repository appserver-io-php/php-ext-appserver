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
        while ($clientSocketResource = stream_socket_accept($this->serverSocketResource)) {
            // dummy read request
            $request = fread($clientSocketResource, 1);
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
            fwrite($clientSocketResource, $responseHeaders . "\r\n" . $responseBody);
            fclose($clientSocketResource);
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
        // init stream context for server connection
        $streamContext = stream_context_create($opts);
        $serverStreamSocketResource = stream_socket_server($localSocket, $errno, $errstr, $flags, $streamContext);
        $serverSocketResource = socket_import_stream($serverStreamSocketResource);

        // set socket options
        //socket_set_option($serverSocketResource, getprotobyname("tcp"), TCP_NODELAY, 1);

        $this->workerCount = $workerCount;
        $this->serverStreamSocketResource = $serverStreamSocketResource;
    }

    public function run()
    {
        $workers = [];
        while (count($workers) < $this->workerCount) {
            $worker = new ThreadWorker($this->serverStreamSocketResource);
            $worker->start();
            $workers[] = $worker;
        }
    }
}

$server = new MultithreadedServer('127.0.0.1:8999', 64, 'BenchmarkSimpleHttpServer/1.0');
$server->start();
