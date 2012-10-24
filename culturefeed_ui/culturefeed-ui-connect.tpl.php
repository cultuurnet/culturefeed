<p>Maak een UiTiD aan en:</p>
<ul>
  <li>ontvang tips op jouw maat</li>
  <li>ontdek wat je vrienden leuk vinden</li>
  <li>blijf als eerste op de hoogte van je favoriete activiteiten</li>
</ul>

<span class="facebook"><?php print $link_facebook ?></span>
<span class="other">OF via <?php print l('Twitter, Google of je e-mailadres', 'culturefeed/oauth/connect/register', array('attributes' => array('class' => array('culturefeedconnect')), 'query' => drupal_get_destination())) ?></span>
