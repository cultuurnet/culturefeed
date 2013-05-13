<h1><?php print $title; ?></h1>

<?php if (!empty($image)): ?>
<img src="<?php print $image; ?>" />
<?php endif; ?>

<?php if ($description): ?>
<p>
  <?php print $description ?>
</p>
<?php endif; ?>

<dl class="clearfix">

  <dt>Leden</dt>
  <dd>

    <?php if ($members): ?>
    <ul>
    <?php foreach ($members as $member): ?>
      <li>
        <a href="<?php print $member['url']; ?>"><?php print $member['name']?></a>
        <?php if (!empty($member['relation'])): ?>(<?php print $member['relation'] ?>)<?php endif; ?>
      </li>
    <?php endforeach; ?>
    </ul>
    <?php endif; ?>

    <?php if (!empty($become_member_link)): ?>
    <?php print $become_member_link ?>
    <?php endif; ?>

  </dd>

  <?php if (!empty($address)): ?>
  <dt>Adres</dt>
  <dd>
    <?php print $address['street']?>, <?php print $address['zip']?> <?php print $address['city']?>
  </dd>
  <?php endif; ?>

  <?php if (!empty($contact)): ?>
  <dt>Contact</dt>
  <dd>
    <?php print implode(' of ', $contact)?>
  </dd>
  <?php endif; ?>

  <?php if ($links): ?>
  <dt>Website</dt>
  <dd>
    <ul>
    <?php foreach ($links as $class => $link): ?>
      <li class="<?php print $class; ?>"><a href="<?php print $link; ?>"><?php print $link; ?></a></li>
    <?php endforeach; ?>
    </ul>
  </dd>
  <?php endif;?>

</dl>

<?php 
print culturefeed_pages_block_request_admin_membership($item);
 ?>