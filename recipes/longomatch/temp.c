/* This source code was produced by mkbundle, do not edit */
#include <mono/metadata/mono-config.h>
#include <mono/metadata/assembly.h>
#include <glib.h>

extern const unsigned char assembly_data_LongoMatchGtk_exe [];
static const MonoBundledAssembly assembly_bundle_LongoMatchGtk_exe = {"LongoMatchGtk.exe", assembly_data_LongoMatchGtk_exe, 7168};
extern const unsigned char system_config;

static const MonoBundledAssembly *bundled [] = {
	&assembly_bundle_LongoMatchGtk_exe,
	NULL
};

static char *image_name = "LongoMatchGtk.exe";

static void install_dll_config_files (void) {

	mono_config_parse_memory (&system_config);

}

static const char *config_dir = NULL;
void mono_mkbundle_init ()
{
	install_dll_config_files ();
	mono_register_bundled_assemblies(bundled);
}
int mono_main (int argc, char* argv[]);

#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif

static char **mono_options = NULL;

static int count_mono_options_args (void)
{
	const char *e = getenv ("MONO_BUNDLED_OPTIONS");
	const char *p, *q;
	int i, n;

	if (e == NULL)
		return 0;

	/* Don't bother with any quoting here. It is unlikely one would
	 * want to pass options containing spaces anyway.
	 */

	p = e;
	n = 1;
	while ((q = strchr (p, ' ')) != NULL) {
		n++;
		p = q + 1;
	}

	mono_options = malloc (sizeof (char *) * (n + 1));

	p = e;
	i = 0;
	while ((q = strchr (p, ' ')) != NULL) {
		mono_options[i] = malloc ((q - p) + 1);
		memcpy (mono_options[i], p, q - p);
		mono_options[i][q - p] = '\0';
		i++;
		p = q + 1;
	}
	mono_options[i++] = strdup (p);
	mono_options[i] = NULL;

	return n;
}


int main (int argc, char* argv[])
{
	char **newargs;
  gchar *exe_path, *mono_path, *db40_path, *lgm_path, *lib_path;
  gchar *addins_path, *mono_lib_path, *etc_path, *gtk_path;
	int i, k = 0;

#ifdef _WIN32
	/* CommandLineToArgvW() might return a different argc than the
	 * one passed to main(), so let it overwrite that, as we won't
	 * use argv[] on Windows anyway.
	 */
	wchar_t **wargv = CommandLineToArgvW (GetCommandLineW (), &argc);
#endif

	newargs = (char **) malloc (sizeof (char *) * (argc + 2) + count_mono_options_args ());

#ifdef _WIN32
	newargs [k++] = g_utf16_to_utf8 (wargv [0], -1, NULL, NULL, NULL);
#else
	newargs [k++] = argv [0];
#endif

	if (mono_options != NULL) {
		i = 0;
		while (mono_options[i] != NULL)
			newargs[k++] = mono_options[i++];
	}

	newargs [k++] = image_name;

	for (i = 1; i < argc; i++) {
#ifdef _WIN32
		newargs [k++] = g_utf16_to_utf8 (wargv [i], -1, NULL, NULL, NULL);
#else
		newargs [k++] = argv [i];
#endif
	}
#ifdef _WIN32
	LocalFree (wargv);
#endif
	newargs [k] = NULL;
	
  exe_path = g_win32_get_package_installation_directory_of_module (NULL);
  lib_path = g_build_filename (exe_path, "lib", NULL);
  etc_path = g_build_filename (exe_path, "etc", NULL);
  mono_lib_path = g_build_filename (exe_path, "lib", "mono", "4.0",
      NULL);
  gtk_path  = g_build_filename (exe_path, "lib", "mono",
      "gtk-sharp-2.0", NULL);
  db40_path = g_build_filename (exe_path, "lib", "mono", "db4o",
      NULL);
  addins_path = g_build_filename (exe_path, "lib", "mono",
      "mono-addins", NULL);
  lgm_path = g_build_filename (exe_path, "lib", "longomatch", NULL);

  mono_path = g_strdup_printf("%s;%s;%s;%s;%s", mono_lib_path,
      gtk_path, db40_path, addins_path, lgm_path);
  g_setenv ("MONO_PATH", mono_path, TRUE);
	mono_set_dirs (lib_path, etc_path);
	
	mono_mkbundle_init();

	return mono_main (k, newargs);
}
