# Sprint 6 - Pruebas integrales y documentación

Este checkpoint cierra el Proyecto 1 de ExtreamFS sin modificar la lógica de producción que ya superó los Sprints 2 a 5.

## Alcance

- Batería integral `backend/tests/test_sprint6.sh`.
- Manual Técnico en PDF y DOCX.
- Manual de Usuario en PDF y DOCX con capturas reales de la interfaz.
- Diagrama de Arquitectura y Diagrama de Flujo.
- Informe de Impacto.
- Plan de Mantenimiento.
- README final con instalación, arquitectura, ejecución y pruebas.

## Aplicar

Desde la raíz del repositorio:

```bash
unzip -o checkpoint_sprint6_pruebas_documentacion.zip -d .
cmake -S backend -B backend/build
cmake --build backend/build
bash backend/tests/test_sprint6.sh
```

Después ejecutar regresión:

```bash
bash backend/tests/test_sprint5.sh
bash backend/tests/test_sprint4.sh
bash backend/tests/test_sprint3.sh
bash backend/tests/test_sprint2.sh
```

## Prueba visual antes de entregar

1. Iniciar backend y frontend.
2. Cargar un `.smia` y ejecutar un flujo completo.
3. Generar y abrir al menos MBR, DISK, TREE y LS.
4. Abrir `docs/Manual_Tecnico_ExtreamFS.pdf` y `docs/Manual_Usuario_ExtreamFS.pdf`.
5. Confirmar que el repositorio privado tiene acceso para el auxiliar correspondiente.

## Archivos para commit

`backend/tests/` puede permanecer ignorado si las pruebas son solo locales. Para la entrega conviene versionar:

```bash
git add README.md SPRINT6_README.md docs/
```

No se deben subir `backend/build/`, `frontend/node_modules/`, `frontend/dist/`, discos `.mia`, reportes temporales ni checkpoints `.zip`.
