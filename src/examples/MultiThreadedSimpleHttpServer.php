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
        $this->maxConnections = 16;
        $this->serverSocketResource = $serverSocketResource;
    }

    /**
     * thread routine
     */
    public function run()
    {
        $this->done = false;
        $connectionCount = 0;

        $dummyData = array();

        do {
            if ($clientSocketResource = @stream_socket_accept($this->serverSocketResource)) {
                ++$connectionCount;
                $dummyData[] = md5(time() . rand(0, PHP_INT_MAX) . time());
                // dummy read request
                $request = fread($clientSocketResource, 1);
                // build up response
                ob_start();
                phpinfo();
                $responseBody = ob_get_clean();
                $responseHeaders =
                    "HTTP/1.1 200 OK" . "\r\n" .
                    "Server: {{server}}" . "\r\n" .
                    "Date: {{date}}" . "\r\n" .
                    "Content-Type: text/html" . "\r\n" .
                    "Content-Length: {{content-length}}" . "\r\n" .
                    "Connection: close" . "\r\n";
                $responseHeaders = str_replace(
                    array('{{server}}', '{{date}}', '{{content-length}}'),
                    array(SERVER_NAME, gmdate('D, d M Y H:i:s T'), strlen($responseBody)),
                    $responseHeaders
                );
                fwrite($clientSocketResource, $responseHeaders . "\r\n" . $responseBody);
                fclose($clientSocketResource);
            }
        } while (1);// ($connectionCount < $this->maxConnections);

        $this->done = true;
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

        while (true) {
            for ($i = 0; $i < count($workers); $i++) {
                if ($workers[$i]->done === true) {
                    echo "Kill ThreadId: " . $workers[$i]->getThreadId() . PHP_EOL;
                    $worker = new ThreadWorker($this->serverStreamSocketResource);
                    $worker->start();
                    $workers[] = $worker;
                    $workers[$i]->kill();
                }
            }
            usleep(100000);
        }
    }
}

$server = new MultithreadedServer('127.0.0.1:8999', 8, 'BenchmarkSimpleHttpServer/1.0');
$server->start();
