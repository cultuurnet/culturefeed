<?php

if (file_exists('vendor/autoload.php')) {
  require_once 'vendor/autoload.php';
}

date_default_timezone_set('Europe/Brussels');

set_include_path(realpath(dirname(__FILE__) . '/..') . PATH_SEPARATOR . get_include_path());

function culturefeed_autoload($class) {
  $file = str_replace('_', '/', $class) . '.php';
  if (FALSE !== stream_resolve_include_path($file)) {
    require_once $file;
  }
  elseif (strncmp('OAuth', $class, 5) === 0) {
    // include the third-party oauth library which is not properly structured to
    // be PSR-0 autoloaded
    require_once dirname(__FILE__) . '/../../OAuth/OAuth.php';
  }
}

spl_autoload_register('culturefeed_autoload');
