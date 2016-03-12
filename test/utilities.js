'use strict';

/**
 * @fileoverview Test for the module "utilities.js"
 * @author doberkofler
 */


/* global describe: false, it:false */


/**
* Module dependencies.
*/

var assert = require('chai').assert;
var utilities = require('../lib/utilities');

var mkdirp = require('mkdirp');
var path = require('path');
var fs = require('fs');


/**
* Module constants.
*/


/**
* Module variables.
*/


/**
* Tests.
*/

describe('utilities.js', function () {

	describe('absoluteFilename', function () {
		describe('when getting the absolute filename', function () {
			it('does return it', function () {
				assert.strictEqual(utilities.absoluteFilename('filename'), path.resolve('lib/filename'));
			});
		});

	});

	describe('fileCopy', function () {
		describe('when copying an existing file', function () {
			var SRC = 'temp/test.old',
				DST = 'temp/test.new';

			mkdirp.sync('temp');

			utilities.fileDelete(SRC);
			utilities.fileDelete(DST);
			fs.writeFileSync(SRC, 'content');

			it('does copy the file', function () {
				utilities.fileCopy(SRC, DST);

				assert.strictEqual(fs.existsSync(DST), true);
			});
		});
	});

});
