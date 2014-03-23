<p><?php print $login_message ?></p>
<p>
  <?php print $login_facebook; ?><br />
  <?php print t('Or via') . ' ' . $login_twitter . ', ' . $login_google . ' ' . t('or') . ' ' . $login_email; ?>
</p>
<p>
  <?php print t("Don't have an UiTiD? Create a !new_account.", array('!new_account' => $register)) ?>
</p>

