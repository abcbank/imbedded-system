cmd_/home/kusw05/imbedded-system/seg7_example/homework/Module.symvers := sed 's/\.ko$$/\.o/' /home/kusw05/imbedded-system/seg7_example/homework/modules.order | scripts/mod/modpost -m -a  -o /home/kusw05/imbedded-system/seg7_example/homework/Module.symvers -e -i Module.symvers   -T -
