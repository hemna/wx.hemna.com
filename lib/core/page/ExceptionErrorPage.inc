<?php
/**
 * This is the main Error page that we show when an exception
 * happens and we aren't in debug mode.
 *
 * @author Walter A. Boring IV
 * @package phpHtmlLib
 *
 */
class ExceptionErrorPage extends open2300Page {

    public function __construct( ) {
        parent::__construct('OOPS!');
    }


    public function request_vars() {
        return array();
    }

    /**
     * Enable caching of this page
     * by default
     */
    public function is_cacheable() {
        return true;
    }

    public function content_block() {

        $c = new DIVtag(array('style' => 'text-align:center;padding:60px;font-size:16px;'));

        $c->add(new SPANtag(array('style' => 'font-size:20pt;color:#aa0000;'), 'Use the Force Luke!'));

        $c->add(Ptag::factory());
        $c->add("An exception happened.");

        return $c;
    }

}

?>