<p><strong><span class="lead"><?php print $total_points; ?> punten</span></strong> <?php print $exchange_link; ?></p>

<p class="muted"><small>Praesent commodo cursus magna, vel scelerisque nisl consectetur et. Nullam quis risus eget urna mollis ornare vel eu leo. Cras justo odio, dapibus ac facilisis in, egestas eget quam. Aenean eu leo quam. Pellentesque ornare sem lacinia quam venenatis vestibulum. <a href="#">Meer informatie</a></small></p>

<table class="table table-striped table-hover">
  <thead>
    <tr>
      <th></th>
      <th></th>
      <th></th>
    </tr>
  </thead>
  <tbody>
    <?php foreach ($items as $item): ?>
      <tr>
        <td class="muted"><small><?php print $item['date']; ?></small></td>
        <td><small><strong><?php print $item['points']; ?></strong></small></td>
        <td><?php print $item['description']; ?></td>
      </tr>
    <?php endforeach; ?>  
  </tbody>
</table>





