<?php

/**
 * @file
 * Contains Drupal\culturefeed_ui\Controller\UserController.
 */

namespace Drupal\culturefeed_ui\Controller;

use Drupal\Core\Controller\ControllerBase;
use Drupal\Core\Url;
use Drupal\culturefeed_ui\Plugin\Block\ProfileMenuBlock;
use Symfony\Component\DependencyInjection\ContainerInterface;
use CultureFeed_User;

/**
 * Class UserController.
 *
 * @package Drupal\culturefeed_ui\Controller
 */
class UserController extends ControllerBase
{
    /**
     * The culturefeed user service.
     *
     * @var CultureFeed_User;
     */
    protected $user;

    /**
     * {@inheritdoc}
     */
    public static function create(ContainerInterface $container) {
        return new static(
            $container->get('culturefeed.current_user')
        );
    }

    /**
     * Constructs a ProfileForm
     *
     * @param CultureFeed_User $user
     */
    public function __construct(
        CultureFeed_User $user
    ) {
        $this->user = $user;
    }

    /**
     * Profile.
     *
     * @return string
     *   Return Hello string.
     */
    public function profile()
    {
        $renderArray = [
            '#theme' => 'culturefeed_ui_profile',
            '#user' => $this->user,
            '#edit_profile_link' => Url::fromRoute('culturefeed_ui.profile_form'),
            '#edit_profile_title' => t('Edit'),
            '#title' => t('My UiTiD')
        ];

        return $renderArray;
    }

}
