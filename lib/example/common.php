<?php

require 'autoloader.php';

require '../OAuth/OAuth.php';

$classLoader = new SplClassLoader(null, '../../lib/CultureFeed');
$classLoader->register();