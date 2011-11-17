<p>Met je eigen UiTID ontvang je <strong>aanbevelingen</strong> op jouw maat, kan je deelnemen aan onze <strong>wedstrijden</strong> en <strong>inschrijven</strong> op onze nieuwsbrief.</p>

<h3>Meld je direct aan via:</h3>

<ul>
  <li class="facebook"><?php print $link_facebook ?></li>
  <li class="twitter"><?php print $link_twitter ?></li>
  <li class="google"><?php print $link_google ?></li>
</ul>

<h3>Geen Facebook, Twitter of Google-account?</h3>

<p><?php print l('Meld je aan', 'culturefeed/oauth/connect', array('attributes' => array('class' => array('culturefeedconnect')), 'query' => drupal_get_destination())) ?> met je UiTID</p>
<p>Of <?php print l('registreer', 'culturefeed/oauth/connect/register', array('attributes' => array('class' => array('culturefeedconnect')), 'query' => drupal_get_destination())) ?> een nieuw UiTID.</p>