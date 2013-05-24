<div id="view-profile">
  <a href="<?php print $user_url; ?>">Bekijk profiel</a>
</div>

<h2>Pagina's waar ik lid van ben</h2>
<?php if (!empty($my_pages)): ?>
  <?php print $my_pages; ?>
<?php else: ?>
  <div class="alert alert-info"><strong>Je bent nog geen lid van een pagina.</strong><br />Zoek (of maak) de pagina van je werkgever en word lid. als je een nieuwe pagina maakt, ben je automatische en lid en beheerder van de pagina.</div>
<?php endif; ?>

<h2>Pagina's die ik volg</h2>
<?php if (!empty($i_follow)): ?>
  <?php print $i_follow; ?>
<?php else: ?>
  <div class="alert alert-info"><strong>Je volgt nog geen pagina's.</strong><br />Zoek en volg pagina's die je interesseren en ontvang een melding als er relevante updates zijn.</div>
<?php endif; ?>

<h2>Zoek nieuwe pagina's:</h2>
<?php print $search_pages; ?>

<div id="page_confirm" class="modal hide fade" tabindex="-1" role="dialog" aria-hidden="true">
  <div class="modal-body"></div>
</div>
