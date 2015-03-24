CREATE OR REPLACE
PACKAGE sample IS

TYPE vc_arr IS TABLE OF VARCHAR2(2000) INDEX BY BINARY_INTEGER;
empty_vc_arr vc_arr;

PROCEDURE pageIndex;

PROCEDURE pageSimple(text IN VARCHAR2 DEFAULT NULL);

PROCEDURE pageFlexible(name_array IN owa.vc_arr, value_array IN owa.vc_arr);

PROCEDURE pageArray(text IN vc_arr DEFAULT empty_vc_arr);

PROCEDURE pageCGI;

PROCEDURE pageCookie;

PROCEDURE pageForm;
PROCEDURE pageFormProcess(name_array IN owa.vc_arr, value_array IN owa.vc_arr);

PROCEDURE pageFileUpload;
PROCEDURE pageFileUploaded(name_array IN owa.vc_arr, value_array IN owa.vc_arr);

PROCEDURE pageBlocking(secs IN NUMBER DEFAULT 60);
PROCEDURE pageDoesBlock(secs IN NUMBER DEFAULT 60);

PROCEDURE pageRedirect;
PROCEDURE pageLocation;
PROCEDURE pageOther;

END sample;
/
