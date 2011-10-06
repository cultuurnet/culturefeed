<?php

  $key = isset($_COOKIE['key']) ? $_COOKIE['key'] : '';
  $secret = isset($_COOKIE['secret']) ? $_COOKIE['secret'] : '';

  $key = isset($_POST['key']) ? $_POST['key'] : $key;
  $secret = isset($_POST['secret']) ? $_POST['secret'] : $secret;

  setcookie('key', $key);
  setcookie('secret', $secret);

?>

<form action="" method="POST">

  <dl>

    <dt><label for="key">Application key</label></dt>
    <dd><input type="text" name="key" value="<?php print $key ?>" /></dd>

    <dt><label for="secret">Application secret</label></dt>
    <dd><input type="text" name="secret" value="<?php print $secret ?>" /></dd>

  </dl>

  <input type="submit" name="submit" value="Submit" />

</form>

<p><a href="index.php">Go back to application</a></p>