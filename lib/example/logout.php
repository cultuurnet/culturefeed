<?php

setcookie('oauth_user', NULL);
setcookie('oauth_token', NULL);
setcookie('oauth_token_secret', NULL);

header('Location: index.php');
exit();