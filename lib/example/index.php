<?php

require 'common.inc';


?>

<?php if (!isset($_COOKIE['oauth_token'])) { ?>
  <p><a href="connect.php">Connect</a></p>
  <?php exit; ?>
<?php } else { ?>
  <p><a href="logout.php">Log out</a></p>
<?php } ?>

<?php

$actions = array();

$actions['updateUser']                    = 'updateUser';
$actions['deleteUser']                    = 'deleteUser';
$actions['getUser']                       = 'getUser';
$actions['searchUsers']                   = 'searchUsers';
$actions['getSimilarUsers']               = 'getSimilarUsers';
$actions['uploadUserDepiction']           = 'uploadUserDepiction';
$actions['resendMboxConfirmationForUser'] = 'resendMboxConfirmationForUser';
$actions['updateUserPrivacy']             = 'updateUserPrivacy';
$actions['getUserServiceConsumers']       = 'getUserServiceConsumers';
$actions['revokeUserServiceConsumer']     = 'revokeUserServiceConsumer';
$actions['getTopEvents']                  = 'getTopEvents';
$actions['getRecommendationsForUser']     = 'getRecommendationsForUser';
$actions['getRecommendationsForEvent']    = 'getRecommendationsForEvent';
$actions['getUrlAddSocialNetwork']        = 'getUrlAddSocialNetwork';
$actions['getUrlChangePassword']          = 'getUrlChangePassword';
$actions['getUrlLogout']                  = 'getUrlLogout';

?>

<form action="" method="get">
  <select name="action" id="action">
    <?php foreach ($actions as $action => $title) : ?>
      <option value="<?php print $action ?>" <?php if ($_GET['action'] == $action) : ?>selected<?php endif ?>><?php print $title ?></option>
    <?php endforeach ?>
  </select>
  <input type="submit" name="submit" value="Submit" />
</form>


<?php

require 'examples.inc';

?>