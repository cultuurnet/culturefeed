<p>Met je eigen UiTid ontvang je <strong>aanbevelingen</strong> op jouw maat, kan je deelnemen aan onze <strong>wedstrijden</strong> en <strong>inschrijven</strong> op onze nieuwsbrief.</p>

<p>Meld je direct aan via:</p>

<ul>
  <li><?php print $link_facebook ?></li>
  <li><?php print $link_twitter ?></li>
  <li><?php print $link_google ?></li>
</ul>

<h3>Geen Facebook, Twitter of Google-gebruiker?</h3>

<p><?php print l('Meld je aan', 'culturefeed/oauth/connect', array('attributes' => array('class' => array('culturefeedconnect')), 'query' => drupal_get_destination())) ?> met je UiTid</p>
<p>Of <?php print l('registreer', 'culturefeed/oauth/connect/register', array('attributes' => array('class' => array('culturefeedconnect')), 'query' => drupal_get_destination())) ?> een nieuw UiTid.</p>