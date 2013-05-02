<div class="followers-block">
  <h3><?php print $title; ?></h3>
  
  <div class="followed-times"><?php print $num_followers ?> x</div>

  <?php if (!empty($followers)): ?>
  
    <ul>
    <?php foreach ($followers as $follower): ?>
      <li>
        <a href="<?print $follower['url'] ?>"><?php print $follower['name']; ?></a>
        <?php if ($follower['picture']): ?>
        <?php print theme('image', array('path' => $follower['picture'] . '?maxwidth=100')) ?>
        <?php endif; ?>
      </li>
    <?php endforeach; ?>
    </ul>

  <?php elseif ($you_follow): ?>
    <p>Je volgt nu deze pagina</p>
  
  <?php else: ?>
    <p>Deze pagina heeft nog geen volgers.</p>
  <?php endif; ?>
  
  <?php 
  /**
   * #follow-PAGEID is used to refresh that part of the html. You can use it
   * freely as you want. E.g. wrap the text above in it or not.
   */
   ?>
  <div id="follow-<?php print $page->getId() ?>">
  <?php if (!$you_follow): ?>
    <?php print $follow_link ?>
  <?php else: ?>
    <?php print $follow_link ?>
  <?php endif; ?>
  </div>

</div>