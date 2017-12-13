<?php
/**
 * @file
 */

namespace Drupal\culturefeed\Element;

use Drupal\Core\Form\FormStateInterface;
use Drupal\file\Element\ManagedFile;
use Drupal\file\FileInterface;

/**
 * Extension of ManagedFile that displays a thumbnail of the uploaded file.
 *
 * @FormElement("managed_image")
 */
class ManagedImage extends ManagedFile
{
  public static function processManagedFile(
    &$element,
    FormStateInterface $form_state,
    &$complete_form
  ) {
    $element = parent::processManagedFile(
      $element,
      $form_state,
      $complete_form
    );

    foreach ($element['fids']['#value'] as $fid) {
      /** @var FileInterface $file */
      $file = $element['file_' . $fid]['filename']['#file'];

      $element['file_' . $fid]['filename']['#theme'] = 'image_style';

      $element['file_' . $fid]['filename'] += array(
        '#uri' => $file->getFileUri(),
        '#style_name' => 'thumbnail',
      );
    }

    return $element;
  }

}
