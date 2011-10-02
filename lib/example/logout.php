<?php

require 'common.inc';

setcookie('oauth_user', NULL);
setcookie('oauth_token', NULL);
setcookie('oauth_token_secret', NULL);

header('Location:' . CULTUREFEED_API_SERVER . 'index.php');
exit;