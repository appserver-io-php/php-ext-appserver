<?php

/**
 * appserver.php
 *
 * NOTICE OF LICENSE
 *
 * This source file is subject to the Open Software License (OSL 3.0)
 * that is available through the world-wide-web at this URL:
 * http://opensource.org/licenses/osl-3.0.php
 */

/**
 * A php test script show functionality of the extension
 *
 * @copyright  	Copyright (c) 2013 <info@techdivision.com> - TechDivision GmbH
 * @license    	http://opensource.org/licenses/osl-3.0.php
 *              Open Software License (OSL 3.0)
 * @author      Johann Zelger <jz@techdivision.com>
 */

header('X-Powered-By: appserver');
header('Location: http://www.google.de');

echo "SAPI_TYPE: " . PHP_SAPI;
echo PHP_EOL . "==========================================". PHP_EOL;

echo "CALL appserver_get_headers()" . PHP_EOL;
echo var_export(appserver_get_headers(), true);
echo PHP_EOL . "==========================================". PHP_EOL;
