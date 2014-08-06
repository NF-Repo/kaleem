<?php

/**
 * @file
 * A single location to store configuration.
 */
global $base_url;
    $twitterappcontent = app_exits_check('twitter');

        foreach ($twitterappcontent as $contenttwitter) {
            $content = $contenttwitter->data;
        }

        $resp = unserialize($content);
        $consumerkey = $resp->twitterconsumerkey;
        $consumersecret=$resp->twittersecretkey;

define('CONSUMER_KEY', $consumerkey);
define('CONSUMER_SECRET', $consumersecret);
define('OAUTH_CALLBACK', $base_url.'/wwctwitteraouthcallback');