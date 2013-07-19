<div class="cnapi-detail">
  
  <div class="detail-middle">
    <div class="detail-left">
      
      <?php if ($shortdescription) : ?>
        <div class="description"><?php print $shortdescription ?></div>
      <?php endif; ?>
      
      <dl>
      
      <?php if ($address) : ?>
        <dt>Waar</dt>
        <dd>
          <?php print $address ?> 
          <?php if ($coords) : ?>
          <div id="show-map"></div>
          <div id="map" class="hidden">
            <a href="http://maps.google.com/maps?hl=nl&amp;q=<?php print $coords['lat'] ?>, <?php print $coords['lng'] ?>"><img src="http://maps.google.com/maps/api/staticmap?center=<?php print $coords['lat'] ?>, <?php print $coords['lng'] ?>&amp;zoom=15&amp;markers=<?php print $coords['lat'] ?>, <?php print $coords['lng'] ?>&amp;size=264x200&amp;sensor=false"></a>
          </div>
          <?php endif; ?> 
        </dd>
      <?php endif; ?>
      
      <?php if ($reservation) : ?>
        <dt>Reserveren</dt>
        <dd>
        <?php if (isset($reservation['mail'])) : ?>
          <?php print $reservation['mail'] ?><br />
        <?php endif; ?>
        <?php if (isset($reservation['phone'])) : ?>
          <?php if (isset($reservation['fax'])) : ?>Telefoon: <?php endif; ?><?php print $reservation['phone'] ?><br />
        <?php endif; ?>
        <?php if (isset($reservation['fax'])) : ?>
          Fax: <?php print $reservation['fax'] ?>
        <?php endif; ?>
        </dd>
      <?php endif; ?>
      
      <?php if ($contact) : ?>
        <dt>Contact</dt>
        <dd>
          <?php print $contact; ?>
        </dd>
      <?php endif; ?>
      
      <?php if ($links) : ?>
        <dt>Links</dt>
        <dd>
          <?php print $links; ?>
        </dd>
      <?php endif; ?>
      
      </dl>
      
      <?php if ($facilities) : ?>
        <dt>Voorzieningen</dt>
        <dd>
          <?php print implode(', ', $facilities); ?>
        </dd>
      <?php endif; ?>

        
      <?php if ($longdescription) : ?>
        <dt>Meer info</dt>
        <dd>
          <div class="description full">
            <?php print $longdescription ?>
          </div>
        </dd>
      <?php endif; ?> 
             
      <?php if ($legend): ?>
        <div class="uitpas-advantages clearfix">
          <?php print $legend; ?>
        </div>
      <?php endif; ?>   
    
    </div>
  
    <div class="detail-right">
      <?php if ($images) : ?>
        <div class="image-holder">
          <?php foreach ($images as $image) : ?>
            <div class="image">
              <?php print $image['image'] ?>
            </div>
            <?php if (!empty($image['copyright'])) : ?><div class="copyright">&copy; <?php print $image['copyright'] ?></div><?php endif; ?>
          <?php endforeach; ?>
        </div>
      <?php endif; ?>       
    </div>
  </div>
  
  <div class="detail-bottom">
  </div>

</div>

