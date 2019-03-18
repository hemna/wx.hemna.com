/**
 * This file uses prototype.js
 * to do it's internal work.
 *
 * @author Walter A. Boring IV
 */

/**
 * send get request and place
 * results into a div.
 * This completely replaces the
 * div contents
 *
 * @param string url
 * @param int divid to replace
 */
function ar_g(url) {
    new Ajax.Request(url, {method:'get',asynchronous:true});
}

/**
 * do a simple ajax request and have
 * the success function process the success
 * and the failure function process the failure
 *
 * @param string url
 * @param string s
 * @param string f
 */
function ar_g_s_f(url, s, f) {
    new Ajax.Request(url, {method:'get',asynchronous:true, onSuccess: s, onFailure: f});
}


/**
 * send get request and place
 * results into a div.
 * This completely replaces the
 * div contents
 *
 * @param string url
 * @param int divid to replace
 */
function ar_g_u(url, id) {
    new Ajax.Updater(id, url, {method:'get',asynchronous:true,evalScripts:true});
}

/**
 * This function makes an ajax request and
 * doesn't replace any div.  The results
 * of the request are passed to message_show()
 *
 * @param string url
 */
function g_message(url) {
    new Ajax.Request(url, {onSuccess:m_s, onFailure:e_s});
}

/**
 * Same as g() except it appends
 * the results to the div.
 *
 * @param string url
 * @param int id
 */
function ar_g_a(url, id) {
    append_div_id = id;
    var ajax = new Ajax.Updater(
         id,
         url,
            {
                method:'get',
                insertion:Insertion.Bottom,
                evalScripts:true
            }
         );
}

/**
 * Do an ajax post and replace the contents
 * of a div with the results
 * @param string id of the form object
 * @param string id to replace
 * @param string message to show?
 */
function ar_p_u(formobj,divid) {
    var ser = formobj.attributes['action'].value;
    new Ajax.Updater(
        divid,
        ser,
        {
            method:'post',
            asynchronous:true,
            evalScripts:true,
            parameters:Form.serialize(formobj)
        }
        );
}

/**
 * Do an ajax post and append the contents
 * to the div with the results
 * @param string id of the form object
 * @param string id to replace
 * @param string message to show?
 */
function ar_p_a(formobj,divid) {
    var ser = formobj.attributes['action'].value;
    new Ajax.Updater(
        divid,
        ser,
        {
            method:'post',
            asynchronous:true,
            evalScripts:true,
            insertion:Insertion.Bottom,
            parameters:Form.serialize(formobj)
        }
        );
}

/**
 * Do an ajax post for a the form engine
 * and replace the contents
 * of a div with the results
 * @param string id of the form object
 * @param string id to replace
 * @param string message to show?
 */
function ar_fp_u(formobj,divid) {
    var ser = formobj.attributes['action'].value;
    new Ajax.Updater(
        divid,
        ser,
        {
            method:'post',
            asynchronous:true,
            evalScripts:true,
            parameters:Form.serialize(formobj)+'&_form_action='+formobj.elements['_form_action'].value
        }
        );
}

/**
 * Do an ajax post for a the form engine
 * and append the div with the results.
 *
 * @param string id of the form object
 * @param string id to replace
 * @param string message to show?
 */
function ar_fp_a(formobj,divid) {
    var ser = formobj.attributes['action'].value;
    new Ajax.Updater(
        divid,
        ser,
        {
            method:'post',
            asynchronous:true,
            evalScripts:true,
            insertion:Insertion.Bottom,
            parameters:Form.serialize(formobj)+'&_form_action='+formobj.elements['_form_action'].value
        }
        );
}


/**
 * show notification message
 *
 */
function message_show(message, time_duration) {
    $('idMessage').innerHTML=message;
    Effect.Appear('idMessage', {queue:'front',duration: 0.1});
    Effect.Fade('idMessage',{queue:'end',duration:time_duration});
}

/**
 * shortcut for the g_message() function
 * This shows a message for 5 seconds.
 *
 * @param XMLHttpRequest
 */
function m_s(message) {
    message_show(message.responseText, 2);
}

/**
 * shortcut for the g_message() function
 * This shows an error message for 5 seconds.
 *
 * @param XMLHttpRequest
 */
function e_s(message) {
    error_show(message.responseText, 2);
}


/**
 * show notification message
 *
 */
function error_show(message, duration) {
    $('idMessage').innerHTML=message;
    $('idMessage').style.visibility='visible';
    $('idMessage').style.backgroundcolor='red';
    $('idMessage').style.color='white';
    $('idMessage').style.opacity=1;

    if (duration != 0) {
        setTimeout('message_hide()', duration*1000);
    }
}
