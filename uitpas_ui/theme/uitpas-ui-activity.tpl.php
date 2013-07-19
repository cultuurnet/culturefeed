<div class="cnapi-detail">

  <div class="detail-top">
      
    <?php if ($headings) : ?> 
      <span class="event-categories">
        <span class="icon"></span> 
        <?php print $headings; ?> 
      </span>
    <?php endif; ?>
    
  </div>
  
  <div class="detail-middle">
    <div class="detail-left">
      
      <?php if ($shortdescription) : ?>
        <div class="description"><?php print $shortdescription ?></div>
      <?php endif; ?>
      
      <dl>
      
      <?php if ($when) : ?>
        <dt>Wanneer</dt>
        <dd>
          <?php print $when ?>
        </dd>
      <?php endif; ?>
 
      <?php if ($where) : ?>
        <dt>Waar</dt>
        <dd>
          <?php print $where; ?> 
        </dd>
      <?php endif; ?>
      
      <?php if ($performers) : ?>
        <dt>Wie</dt>
        <dd>
          <?php print implode(', ', $performers); ?>
        </dd>
      <?php endif; ?>
      
      <?php if ($price) : ?>
        <dt>Prijs</dt>
        <dd>
          <?php print $price ?>
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
      
      <?php if ($tickets) : ?>
        <dt></dt>
        <dd>
          <?php print implode(', ', $tickets); ?>
        </dd>
      <?php endif; ?>
      
      <?php if ($organiser && !$location_is_organiser) : ?>
        <dt>Organisatie</dt>
        <dd>
          <?php print $organisation ?>
        </dd>
      <?php endif; ?>
      
      <?php if ($contact) : ?>
        <dt>Contact</dt>
        <dd>
        <?php if (isset($contact['mail'])) : ?>
          <?php print $contact['mail'] ?><br />
        <?php endif; ?>
        <?php if (isset($contact['phone'])) : ?>
          <?php if (isset($contact['fax'])) : ?>Telefoon: <?php endif; ?><?php print $contact['phone'] ?><br />
        <?php endif; ?>
        <?php if (isset($contact['fax'])) : ?>
          Fax: <?php print $contact['fax'] ?>
        <?php endif; ?>
        </dd>
      <?php endif; ?>
      
      <?php if ($links) : ?>
        <dt>Links</dt>
        <dd>
          <?php print $links; ?>
        </dd>
      <?php endif; ?>
      
      <?php if ($agefrom) : ?>
        <dt>Leeftijd</dt>
        <dd>
          <?php print $agefrom; ?>
          <?php if ($for_children) : ?>
            <?php print $vlieg_image ?>
          <?php endif; ?>
        </dd>
      <?php endif; ?>
        
      <?php if ($targetaudiences) : ?>
        <dt>Doelgroep</dt>
        <dd>
          <?php print implode(', ', $targetaudiences); ?>
        </dd>
      <?php endif; ?>
      
      <?php if ($facilities) : ?>
        <dt>Voorzieningen</dt>
        <dd>
          <?php print implode(', ', $facilities); ?>            
        </dd>
      <?php endif; ?>
      
      <?php if ($longdescription) : ?>
        <dt>Meer info</dt>
        <dd>
          <?php print $longdescription ?>          
        </dd>
      <?php endif; ?>
      
      </dl>
    
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
      <?php if ($icons): ?>
        <div class="uitpas-advantages">
          <?php print $icons; ?>
        </div>       
      <?php endif; ?>            
    </div>
  </div>
  
  <div class="detail-bottom">
  </div>
  
</div>