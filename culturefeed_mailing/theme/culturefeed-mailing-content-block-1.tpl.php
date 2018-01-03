<?php
/**
 * @file
 * Template for first block in mailing.
 */
?>

<tr>
  <td valign="top">
    <table border="0" cellpadding="0" cellspacing="0">
      <tbody>
        <tr>
          <td valign="top" width="115">
            <a href="<?php print $url_image; ?>" title="<?php print $title; ?>"><?php print $image; ?></a>
          </td>
          <td valign="top" style="text-align:left;">
            <a href="<?php print $url_title; ?>" style="color: #000001; font-family: Arial, sans-serif; font-size: 18px; font-weight: bold; line-height: 120%; text-align: left; text-decoration:none;">
              <?php print $title; ?>
            </a>
            <br>
            <span style="color: #000000; font-family: Arial, sans-serif; font-size: 13px; font-weight: normal; line-height: 120%; text-align: left;">
              <?php print $intro; ?>
            </span>
            <br>
            <a href="<?php print $url_readon; ?>" style="color: #F10B0D; font-family: Arial, sans-serif; font-size: 13px; font-weight: normal; line-height: 200%; text-decoration: none; border-bottom: 1px dotted #F10B0D;">
              <?php print $link_title; ?>
            </a>
          </td>
        </tr>
      </tbody>
    </table>
  </td>
</tr>
<tr>
    <td height="10" style="LINE-HEIGHT: 1px; FONT-SIZE: 1px;"><?php print $spacer; ?>
    </td>
</tr>
<tr>
    <td bgcolor="#efeeec" height="1" width="600"></td>
</tr>
<tr>
    <td height="10" style="LINE-HEIGHT: 1px; FONT-SIZE: 1px;"><?php print $spacer; ?>
    </td>
</tr>
