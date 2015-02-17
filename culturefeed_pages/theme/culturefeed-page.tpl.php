<?php if ($cover): ?>
  <img src="<?php print $cover ?>?width=690&height=200&crop=auto" />
<?php endif; ?>

<?php if ($baseline): ?>
<p>
  <?php print $baseline ?>
</p>
<?php endif; ?>

<?php if ($description): ?>
<p>
  <?php print $description ?>
</p>
<?php endif; ?>

<dl>

  <?php if (!empty($address)): ?>
  <dt><?php print t('Address'); ?></dt>
  <dd>
    <address>
    <?php print $address['street']?>, <br />
    <?php print $address['zip']?> <?php print $address['city']?>
    </address>
  </dd>
  <?php endif; ?>

  <?php if (!empty($contact)): ?>
  <dt><?php print t('Contact'); ?></dt>
  <dd>
    <ul>
      <li>
        <?php print implode(' ' . t('or') . ' ', $contact)?>
      </li>
    </ul>
  </dd>
  <?php endif; ?>

  <?php if ($links): ?>
  <dt><?php print t('Website'); ?></dt>
  <dd>
    <ul>
    <?php foreach ($links as $link): ?>
      <li><a href="<?php print $link; ?>"><?php print $link; ?></a></li>
    <?php endforeach; ?>
    </ul>
  </dd>
  <?php endif;?>

  <dt><?php print t('Members'); ?></dt>
  <dd>
    <?php if ($members): ?>
    <ul>
    <?php foreach ($members as $member): ?>
      <li>
        <a href="<?php print $member['url']; ?>"><?php print $member['name']?></a>
        <?php if (!empty($member['relation'])): ?> - <?php print $member['relation'] ?><?php endif; ?>
      </li>
    <?php endforeach; ?>
    </ul>
    <?php endif; ?>

    <?php if (!empty($become_member_link)): ?>
    <?php print $become_member_link ?>
    <?php endif; ?>
  </dd>

</dl>

<?php if ($image): ?>
  <img src="<?php print $image ?>?maxwidth=240&maxheight=240" />
<?php endif; ?>

<?php
print culturefeed_pages_block_request_admin_membership($item);
 ?>
