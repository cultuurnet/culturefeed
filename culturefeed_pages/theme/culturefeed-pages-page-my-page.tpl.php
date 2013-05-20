<div id="view-profile">
  <a href="<?php print $user_url; ?>">Bekijk profiel</a>
</div>

<?php if (!empty($my_pages)): ?>
<h2>Pagina's waar ik lid van ben</h2>
<?php print $my_pages; ?>
<?php endif; ?>

<?php if (!empty($i_follow)): ?>
<h2>Pagina's die ik volg</h2>
<?php print $i_follow; ?>
<?php endif; ?>

<h2>Zoek nieuwe pagina's:</h2>
<?php print $search_pages; ?>

<div id="page_confirm" class="modal hide fade" tabindex="-1" role="dialog" aria-hidden="true">
  <div class="modal-body"></div>
</div>
